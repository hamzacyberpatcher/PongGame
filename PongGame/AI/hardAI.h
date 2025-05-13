#pragma once
#include "../windows/BasicPongGame.h"

class PongGameAiHard : public BasicPongGame {
    
    float aiVelocity = 0.0f;
    float aiReactionTimer = 0.0f;
    const float AI_REACTION_DELAY = 0.08f; // 80 ms reaction delay
public:
    void moveP2(float dt)
    {
        aiReactionTimer += dt;
        // Smart AI movement
        RectangleShape aiRect = m_p2.getRect();
        Vector2d ballPos = m_ball.getPosition();
        Vector2d ballVel = m_ball.getVelocity();

        if (ballVel.x > 0 && aiReactionTimer >= AI_REACTION_DELAY) {
            float predictedY = simulateWallBounces(ballPos, ballVel, aiRect.x);

            float aiCenterY = aiRect.y + aiRect.height / 2;

            float diff = predictedY - aiCenterY;

            if (std::abs(diff) > 5.0f) {
                aiVelocity = (diff > 0 ? 1 : -1) * PADDLE_SPEED;
            }
            else {
                aiVelocity = 0.0f;
            }

            m_p2.moveY(aiVelocity * dt);
        }
    }

    float simulateWallBounces(Vector2d pos, Vector2d vel, float targetX)
    {
        float time = (targetX - pos.x) / vel.x;
        float predictedY = pos.y + vel.y * time;

        // Reflect vertically if it goes out of bounds (simulate bounce)
        while (predictedY < 0 || predictedY > HEIGHT) {
            if (predictedY < 0)
                predictedY = -predictedY;
            else if (predictedY > HEIGHT)
                predictedY = 2 * HEIGHT - predictedY;
        }

        return predictedY;
    }

    void paddleCollision()
    {
        // Paddle collisions
        RectangleShape r1 = m_p1.getRect();
        RectangleShape r2 = m_p2.getRect();
        Vector2d b = m_ball.getPosition();

        if (b.x - m_ball.getRadius() < r1.x + r1.width &&
            b.x + m_ball.getRadius() > r1.x &&
            b.y > r1.y && b.y < r1.y + r1.height) {
            m_ball.setVelocity(Vector2d(std::abs(m_ball.getVelocity().x), m_ball.getVelocity().y));
            aiReactionTimer = 0.0f;
        }

        if (b.x + m_ball.getRadius() > r2.x &&
            b.x - m_ball.getRadius() < r2.x + r2.width &&
            b.y > r2.y && b.y < r2.y + r2.height) {
            m_ball.setVelocity(Vector2d(-std::abs(m_ball.getVelocity().x), m_ball.getVelocity().y));
            aiReactionTimer = 0.0f;
        }

        // Scoring
        if (b.x < 0) {
            scoreP2++;
            m_ball.setPosition(Vector2d(WIDTH / 2, HEIGHT / 2));
            m_ball.setVelocity(Vector2d(BALL_SPEED, BALL_SPEED));
        }
        if (b.x > WIDTH) {
            scoreP1++;
            m_ball.setPosition(Vector2d(WIDTH / 2, HEIGHT / 2));
            m_ball.setVelocity(Vector2d(BALL_SPEED, BALL_SPEED));
        }
    }
};