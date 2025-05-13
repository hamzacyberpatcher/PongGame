#pragma once
#include "utility.h"
#include "vars.h"

class Ball {
    Vector2d m_position;
    Vector2d m_velocity;
    float m_radius;

public:
    Ball(Vector2d pos, float radius)
        : m_position(pos), m_velocity(BALL_SPEED, BALL_SPEED), m_radius(radius) {
    }

    Vector2d getPosition() const { return m_position; }
    Vector2d getVelocity() const { return m_velocity; }
    float getRadius() const { return m_radius; }

    void setPosition(Vector2d pos) { m_position = pos; }
    void setVelocity(Vector2d vel) { m_velocity = vel; }

    void update(float dt) {
        m_position.x += m_velocity.x * dt;
        m_position.y += m_velocity.y * dt;

        // Bounce off top and bottom
        if (m_position.y - m_radius < 0 || m_position.y + m_radius > HEIGHT)
            m_velocity.y = -m_velocity.y;
    }
};