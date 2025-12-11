#include <cmath>
#include "../../Header/Game/Person.h"

Person::Person()
    : xPos(0.0f), yPos(0.0f), rotTan(0.0f), isSick(false), hasSeatbelt(false) {
}

Person::Person(float x, float y)
    : xPos(x), yPos(y), rotTan(0.0f), isSick(false), hasSeatbelt(false) {
}

float Person::getXPos() const {
    return xPos;
}

float Person::getYPos() const {
    return yPos;
}

float Person::getRotationTangent() const {
    return rotTan;
}

float Person::getRotationAngle() const {
    return std::atan(rotTan);
}

bool Person::getIsSick() const {
    return isSick;
}

bool Person::getHasSeatbelt() const {
    return hasSeatbelt;
}

void Person::setXPos(float x) {
    xPos = x;
}

void Person::setYPos(float y) {
    yPos = y;
}

void Person::setRotationTangent(float tan) {
    rotTan = tan;
}

void Person::setIsSick(bool sick) {
    isSick = sick;
}

void Person::setHasSeatbelt(bool seatbelt) {
    hasSeatbelt = seatbelt;
}