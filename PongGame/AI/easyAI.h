#pragma once
#include <string>
#include "../essentials/ball.h"
#include "../essentials/paddle.h"
#include "../essentials/vars.h"
#include "../essentials/utility.h"
#include "../windows/BasicPongGame.h"

class PongGameAiEasy : public BasicPongGame {
    

public:
    void moveP2(float dt)
    {
        // Simple AI logic (follows the ball slowly)
        RectangleShape aiRect = m_p2.getRect();
        float aiCenter = aiRect.y + aiRect.height / 2;
        float ballY = m_ball.getPosition().y;

        if (ballY < aiCenter - 10)
            m_p2.moveY(-PADDLE_SPEED * dt);
        else if (ballY > aiCenter + 10)
            m_p2.moveY(PADDLE_SPEED * dt);
    }
};
