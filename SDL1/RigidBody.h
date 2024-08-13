#pragma once

#include "Vector2D.h"

#define GRAVITY 9.8f
#define MASS 1.0f

class RigidBody {
public:
	RigidBody() { };

	void ApplyForceX(float Fx) {
		m_Force.X = Fx;
	}

	void Update(float dt) {
		m_Acceleration = Vector2D(m_Force.X * (1.0f / m_Mass), m_Gravity + m_Force.Y * (1/m_Mass));

		m_Velocity = m_Velocity + m_Acceleration * dt;

		m_Position = m_Position + m_Velocity * dt;

	}

	Vector2D GetPosition() const { return m_Position; }

	void UnSetForce() {
		m_Force = Vector2DNull;
	}

private:
	float m_Mass{ MASS };
	float m_Gravity{ GRAVITY };

	Vector2D m_Force;
	
	Vector2D m_Velocity;
	Vector2D m_Acceleration;
	Vector2D m_Position;
};
