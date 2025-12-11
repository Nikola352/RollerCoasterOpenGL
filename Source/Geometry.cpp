#include "../Header/Geometry.h"

Vec2::Vec2() : x(0.0f), y(0.0f) {}
Vec2::Vec2(float x, float y) : x(x), y(y) {}

Vec2 Vec2::operator+(const Vec2& other) const {
    return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator-(const Vec2& other) const {
    return Vec2(x - other.x, y - other.y);
}

Vec2 Vec2::operator*(float scalar) const {
    return Vec2(x * scalar, y * scalar);
}

float Vec2::length() const {
    return sqrtf(x * x + y * y);
}

Vec2 Vec2::normalized() const {
    float len = length();
    if (len > 0.0001f) {
        return Vec2(x / len, y / len);
    }
    return Vec2(0.0f, 0.0f);
}

Vec2 rotate(Vec2 vec, float angle) {
    float c = cos(angle), s = sin(angle);
    return Vec2(
        c * vec.x - s * vec.y,
        s * vec.x + c * vec.y
    );
}