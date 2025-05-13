#pragma once
#include <string>
#include "essentials/ball.h"
#include "essentials/paddle.h"
#include "essentials/vars.h"
#include "essentials/utility.h"
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
};