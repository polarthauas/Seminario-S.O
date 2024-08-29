#pragma once

class Vector2D {
public:
    float x, y;

    Vector2D(float x=0.0f, float y=0.0f) : x(x), y(y) {}

    Vector2D operator+(const Vector2D& v) const {
        return Vector2D(x + v.x, y + v.y);
    }

    Vector2D operator-(const Vector2D& v) const {
        return Vector2D(x - v.x, y - v.y);
    }

    Vector2D operator*(float scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    void add(const Vector2D& v) {
        x += v.x;
        y += v.y;
    }

    void scale(float scalar) {
        x *= scalar;
        y *= scalar;
    }
};

const Vector2D Vector2DNull = Vector2D(0.0f, 0.0f);
