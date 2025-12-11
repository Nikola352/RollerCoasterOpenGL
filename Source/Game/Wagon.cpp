#include <cmath>
#include <stdexcept>
#include <vector>
#include "../../Header/Game/Wagon.h"
#include "../../Header/Game/Person.h"
#include "../../Header/Geometry.h"

const std::vector<Vec2> passengerDelta({Vec2(0.02666667f, 0.04f), Vec2(-0.0355555556, 0.04f) });

Wagon::Wagon(float x, float y) : xPos(x), yPos(y), rotTan(0.0f) {
}

float Wagon::getXPos() const {
    return xPos;
}

float Wagon::getYPos() const {
    return yPos;
}

float Wagon::getRotationTangent() const {
    return rotTan;
}

float Wagon::getRotationAngle() const {
    return std::atan(rotTan);
}

Vec2 Wagon::calculatePassengerPosition(size_t index) const {
    Vec2 delta = passengerDelta[index];
    delta = rotate(delta, getRotationAngle());
    delta.x *= 0.5625; // screen ration correction
    return Vec2(xPos, yPos) + delta;
}

void Wagon::setXPos(float x) {
    for (Person& person : getPassengers()) {
        person.setXPos(person.getXPos() + x - xPos);
    }
    xPos = x;
}

void Wagon::setYPos(float y) {
    for (Person& person : getPassengers()) {
        person.setYPos(person.getYPos() + y - yPos);
    }
    yPos = y;
}

void Wagon::setRotationTangent(float tan) {
    rotTan = tan;
    for (size_t i = 0; i < passengers.size(); i++) {
        Vec2 pos = calculatePassengerPosition(i);
        passengers[i].setXPos(pos.x);
        passengers[i].setYPos(pos.y);
        passengers[i].setRotationTangent(tan);
    }
}

void Wagon::addPassenger(Person& person) {
    if (passengers.size() >= 2) {
        throw std::logic_error("Wagon is full: cannot add more than 2 passengers");
    }
    if (passengers.empty()) {   // front passenger
        Vec2 pos = calculatePassengerPosition(0);
        person.setXPos(pos.x);
        person.setYPos(pos.y);
    }
    else {                      // back passenger
        Vec2 pos = calculatePassengerPosition(1);
        person.setXPos(pos.x);
        person.setYPos(pos.y);
    }
    person.setRotationTangent(rotTan);
    passengers.push_back(person);
}

std::vector<Person>& Wagon::getPassengers() {
    return passengers;
}

const std::vector<Person>& Wagon::getPassengers() const {
    return passengers;
}