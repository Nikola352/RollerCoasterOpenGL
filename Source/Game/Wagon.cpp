#include <cmath>
#include "../../Header/Game/Wagon.h"

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

void Wagon::setXPos(float x) {
    xPos = x;
}

void Wagon::setYPos(float y) {
    yPos = y;
}

void Wagon::setRotationTangent(float tan) {
    rotTan = tan;
}