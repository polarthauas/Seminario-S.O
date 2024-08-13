#pragma once

class Vector2D {
public:
	Vector2D(float x = 0.0f, float y = 0.0f	)
		: X(x), Y(y) {}

	float X, Y;

	Vector2D operator +(const Vector2D& v2) const { return Vector2D(X + v2.X, Y + v2.Y); }
	Vector2D operator -(const Vector2D& v2) const { return Vector2D(X - v2.X, Y - v2.Y); }
	Vector2D operator *(const float s) const { return Vector2D(X * s, Y * s); }
	
	bool operator ==(const Vector2D& v2) const { return X == v2.X && Y == v2.Y; }
	bool operator !=(const Vector2D& v2) const { return !(*this == v2); }
};

const auto Vector2DNull = Vector2D(0.0f, 0.0f);
