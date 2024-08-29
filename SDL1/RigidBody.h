#pragma once

#include "Vector2D.h"


class RigidBody {
public:
    Vector2D position;
    Vector2D velocity;
    Vector2D acceleration;
    Vector2D force;

    Vector2D oldPosition;

    float gravity;
    float maxFallSpeed; 

    float mass;

    RigidBody() : gravity(20.5f), maxFallSpeed(100.0f), mass(1.0f) {}

    inline void applyGravity() {
        applyForceY(gravity * mass);
    }

    inline void applyForceY(const float Fy) {
        force.y += Fy;
    }

    inline void applyForceX(const float Fx) {
        force.x += Fx;
    }

    inline void setVelocity(const Vector2D& vel) {
        velocity = vel;
    }

    inline void setVelocityX(float velX) {
        velocity.x = velX;
    }

    inline void setVelocityY(float velY) {
        velocity.y = velY;
    }
    
    inline void applyForce(const Vector2D& f) {
        force.add(f);
    }
    
    void update(float deltaTime) {
        acceleration = force * (1 / mass);

        velocity.add(acceleration);
        if (velocity.y > maxFallSpeed) {
            velocity.y = maxFallSpeed;
        }

        position.add(velocity * deltaTime);
        force = Vector2DNull;
    }
};

