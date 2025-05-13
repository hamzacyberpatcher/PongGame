#pragma once
#include "../windows/BasicPongGame.h"

class PongGameHuman : public BasicPongGame {
public:
    PongGameHuman() {}

    void moveP2(float dt)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            m_p2.moveY(-PADDLE_SPEED * dt);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            m_p2.moveY(PADDLE_SPEED * dt);
    }
};