#pragma once
#include <vector>
#include "Wagon.h"

class Train {
private:
    float frontXPos;
    float velocity;
    float acceleration;
    bool constantAccelMode; // If true acceleration does not change. If false, it changes according to Track.
    std::vector<Wagon> wagons;

    float getAverageSlope() const;
    void updateWagonPositions();
    bool isWagonFull(size_t index) const;
public:
    // Constructors
    Train();
    Train(float x, float velocity = 0.0f, float acceleration = 0.0f, bool constAccelMode = false);

    // Getters
    float getFrontXPos() const;
    float getVelocity() const;
    float getAcceleration() const;
    const std::vector<Wagon>& getWagons() const;
    std::vector<Wagon>& getWagons();
    size_t getWagonCount() const;
    bool isEmpty() const;

    // Setters
    void setFrontXPos(float x);
    void setVelocity(float vel);
    void setAcceleration(float accel);
    void setConstantAccelMode(bool constantAccelMode);

    // Wagon management
    void addWagons(size_t count);
    Wagon& getWagon(size_t index);
    const Wagon& getWagon(size_t index) const;

    // Passenger management
    std::vector<Person*> getAllPassengers();
    std::vector<const Person*> getAllPassengers() const;
    void addNewPassenger();
    Person& getPassenger(size_t index);
    const Person& getPassenger(size_t index) const;

    // High-level operations
    void stop();
    void setConstantVelocity(float velocity);
    void start(float accel);

    // Physics/Movement
    void update(float deltaTime);

    // Utility
    size_t getTotalPassengerCount() const;
};