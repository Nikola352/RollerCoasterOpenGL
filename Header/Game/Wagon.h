#pragma once
#include <vector>
#include "Person.h"
#include "../Geometry.h"

class Wagon {
private:
	float xPos;
	float yPos;
	float rotTan;
	std::vector<Person> passengers;

	Vec2 calculatePassengerPosition(size_t index) const;
public:
	Wagon();
	Wagon(float x, float y);

	float getXPos() const;
	float getYPos() const;
	float getRotationTangent() const;
	float getRotationAngle() const;

	void setXPos(float x);
	void setYPos(float y);
	void setRotationTangent(float tan);

	void addPassenger(Person& person);
	std::vector<Person>& getPassengers();
	const std::vector<Person>& getPassengers() const;
};