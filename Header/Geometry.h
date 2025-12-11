#pragma once
#include <cmath>

struct Vec2 {
    float x, y;

    Vec2();
    Vec2(float x, float y);

    Vec2 operator+(const Vec2& other) const;

    Vec2 operator-(const Vec2& other) const;

    Vec2 operator*(float scalar) const;

    float length() const;

    Vec2 normalized() const;
};

Vec2 rotate(Vec2 vec, float angle);