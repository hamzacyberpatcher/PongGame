#pragma once
#include <string>
#include "essentials/ball.h"
#include "essentials/paddle.h"
#include "essentials/vars.h"
#include "essentials/utility.h"
#include "../windows/BasicPongGame.h"

class PongGameAiMid : public BasicPongGame {
public:
    void moveP2(float dt)
    {
        RectangleShape aiRect = m_p2.getRect();
        Vector2d ballPos = m_ball.getPosition();
        Vector2d ballVel = m_ball.getVelocity();

        // Only move when the ball is approaching the AI
        if (ballVel.x > 0) {
            // Predict where the ball will intersect with AI's X
            float timeToReachAI = (aiRect.x - ballPos.x) / ballVel.x;
            float predictedY = ballPos.y + ballVel.y * timeToReachAI;

            // Clamp predicted Y to screen
            predictedY = std::max(0.f, std::min((float)HEIGHT, predictedY));

            float aiCenter = aiRect.y + aiRect.height / 2;

            if (predictedY < aiCenter - 10)
                m_p2.moveY(-PADDLE_SPEED * dt);
            else if (predictedY > aiCenter + 10)
                m_p2.moveY(PADDLE_SPEED * dt);
        }
    }
};