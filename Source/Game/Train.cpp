#include <stdexcept>
#include <cmath>
#include <algorithm>
#include "../../Header/Game/Train.h"
#include "../../Header/Game/Track.h"

// Constructors
Train::Train()
    : frontXPos(0.0f), velocity(0.0f), acceleration(0.0f), constantAccelMode(false) {
}

Train::Train(float x, float velocity, float acceleration, bool constAccelMode)
    : frontXPos(x), velocity(velocity), acceleration(acceleration), constantAccelMode(constAccelMode) {
}

// Getters
float Train::getFrontXPos() const {
    return frontXPos;
}

float Train::getVelocity() const {
    return velocity;
}

float Train::getAcceleration() const {
    return acceleration;
}

const std::vector<Wagon>& Train::getWagons() const {
    return wagons;
}

std::vector<Wagon>& Train::getWagons() {
    return wagons;
}

size_t Train::getWagonCount() const {
    return wagons.size();
}

bool Train::isEmpty() const {
    return wagons.empty();
}

// Setters
void Train::setFrontXPos(float x) {
    frontXPos = x;
    updateWagonPositions();
}

void Train::setVelocity(float vel) {
    velocity = vel;
}

void Train::setAcceleration(float accel) {
    acceleration = accel;
}

void Train::setConstantAccelMode(bool constMode) {
    constantAccelMode = constMode;
}

// Wagon management
void Train::addWagons(size_t count) {
    for (size_t i = 0; i < count; i++) {
        wagons.push_back(Wagon());
    }
    updateWagonPositions();
}

Wagon& Train::getWagon(size_t index) {
    if (index >= wagons.size()) {
        throw std::out_of_range("Wagon index out of range");
    }
    return wagons[index];
}

const Wagon& Train::getWagon(size_t index) const {
    if (index >= wagons.size()) {
        throw std::out_of_range("Wagon index out of range");
    }
    return wagons[index];
}

bool Train::isWagonFull(size_t index) const {
    if (index >= wagons.size()) {
        throw std::out_of_range("Wagon index out of range");
    }
    return wagons[index].getPassengers().size() >= 2;
}

// Passenger management
std::vector<Person*> Train::getAllPassengers() {
    std::vector<Person*> allPassengers;
    for (Wagon& wagon : wagons) {
        for (Person& person : wagon.getPassengers()) {
            allPassengers.push_back(&person);
        }
    }
    return allPassengers;
}

std::vector<const Person*> Train::getAllPassengers() const {
    std::vector<const Person*> allPassengers;
    for (const Wagon& wagon : wagons) {
        for (const Person& person : wagon.getPassengers()) {
            allPassengers.push_back(&person);
        }
    }
    return allPassengers;
}

void Train::addNewPassenger() {
    // Find the first wagon that has space
    for (Wagon& wagon : wagons) {
        if (wagon.getPassengers().size() < 2) {
            Person newPerson;
            wagon.addPassenger(newPerson);
            return;
        }
    }

    // If no wagon has space, throw an exception
    throw std::logic_error("No wagon has space for a new passenger");
}

Person& Train::getPassenger(size_t index) {
    size_t wagonIndex = index / 2;
    size_t passengerIndex = index % 2;

    if (wagonIndex >= wagons.size()) {
        throw std::out_of_range("Passenger index out of range (wagon doesn't exist)");
    }

    Wagon& wagon = wagons[wagonIndex];
    if (passengerIndex >= wagon.getPassengers().size()) {
        throw std::out_of_range("Passenger index out of range (passenger doesn't exist in wagon)");
    }

    return wagon.getPassengers()[passengerIndex];
}

const Person& Train::getPassenger(size_t index) const {
    size_t wagonIndex = index / 2;
    size_t passengerIndex = index % 2;

    if (wagonIndex >= wagons.size()) {
        throw std::out_of_range("Passenger index out of range (wagon doesn't exist)");
    }

    const Wagon& wagon = wagons[wagonIndex];
    if (passengerIndex >= wagon.getPassengers().size()) {
        throw std::out_of_range("Passenger index out of range (passenger doesn't exist in wagon)");
    }

    return wagon.getPassengers()[passengerIndex];
}

bool Train::isSeatTaken(size_t index) const {
    size_t wagonIndex = index / 2;
    size_t passengerIndex = index % 2;

    if (wagonIndex >= wagons.size()) {
        return false;
    }

    const Wagon& wagon = wagons[wagonIndex];
    if (passengerIndex >= wagon.getPassengers().size()) {
        return false;
    }

    return true;
}

void Train::removePassenger(size_t index) {
    size_t wagonIndex = index / 2;
    size_t passengerIndex = index % 2;

    if (wagonIndex >= wagons.size()) {
        throw std::out_of_range("Passenger index out of range (wagon doesn't exist)");
    }

    Wagon& wagon = wagons[wagonIndex];
    if (passengerIndex >= wagon.getPassengers().size()) {
        throw std::out_of_range("Passenger index out of range (passenger doesn't exist in wagon)");
    }

    wagon.removePassenger(passengerIndex);
}

// High-level operations
void Train::stop() {
    constantAccelMode = true;
    acceleration = 0;
    velocity = 0;
}

void Train::setConstantVelocity(float velocity) {
    constantAccelMode = true;
    acceleration = 0;
    this->velocity = velocity;
}

void Train::start(float accel) {
    constantAccelMode = true;
    acceleration = accel;
    velocity = 0;
}

// Physics/Movement
float Train::getAverageSlope() const {
    if (wagons.empty()) {
        return 0.0f;
    }
    float sum = 0.0f;
    for (const Wagon& wagon : wagons) {
        sum += getTrackSlopeAt(wagon.getXPos());
    }
    return sum / wagons.size();
}

static float calcCos(float tg) {
    return std::max(1.0f / sqrtf(1 + tg * tg), 0.05f);
}

void Train::update(float deltaTime) {
    float avgSlope = getAverageSlope();

    if (!constantAccelMode) {
        // Update acceleration according to current average track slope
        //if (fabs(avgSlope) < 1e-5) {
        //    acceleration = 0.0f;
        //}
        //else if (avgSlope > 0) {
        //    acceleration = -0.15f;
        //}
        //else {
        //    acceleration = 0.15f;
        //}
        acceleration = -0.08f * avgSlope;
        if (acceleration > 0.5f) acceleration = 0.5f;
        if (acceleration < -0.45f) acceleration = -0.45f;
    }

    velocity += acceleration * deltaTime;

    frontXPos += velocity * calcCos(avgSlope) * deltaTime;

    updateWagonPositions();
}

void Train::updateWagonPositions() {
    const float desiredDistance = 0.08f;
    float currentXPos = frontXPos;

    for (Wagon& wagon : wagons) {
        float x = currentXPos;
        float y = getTrackHeightAt(x);
        float slope = getTrackSlopeAt(x);

        // Adjust X-delta based on slope to maintain consistent arc length
        float xDelta = desiredDistance * calcCos(slope);

        if (fabs(slope) < 1e-5) {
            wagon.setXPos(x);
            wagon.setYPos(y + 0.025f);
            wagon.setRotationTangent(0.0f);
        }
        else {
            float tg = 1.0f / slope;
            float s = tg / sqrtf(1 + tg * tg), c = 1.0f / sqrtf(1 + tg * tg);
            float offsetX = 0.025f * c;
            float offsetY = 0.025f * s;
            if (slope > 0) {
                offsetX = -offsetX;
                offsetY = -offsetY;
            }
            wagon.setXPos(x + offsetX);
            wagon.setYPos(y + offsetY);
            wagon.setRotationTangent(slope);
        }

        currentXPos -= xDelta;
    }
}

// Utility
size_t Train::getTotalPassengerCount() const {
    size_t count = 0;
    for (const Wagon& wagon : wagons) {
        count += wagon.getPassengers().size();
    }
    return count;
}