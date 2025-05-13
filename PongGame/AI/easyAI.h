#pragma once
#include <string>
#include "essentials/ball.h"
#include "essentials/paddle.h"
#include "essentials/vars.h"
#include "essentials/utility.h"
#include "windows/gamewindow.h"


class PongGameAiEasy : public GameWindow {
    Ball m_ball;
    Paddle m_player;
    Paddle m_ai;
    sf::Clock clock;
    int scorePlayer;
    int scoreAi;

public:
    PongGameAiEasy()
        : m_ball(Vector2d(WIDTH / 2, HEIGHT / 2), 15),
        m_player(RectangleShape(50, HEIGHT / 2 - 50, 15, 100)),
        m_ai(RectangleShape(WIDTH - 65, HEIGHT / 2 - 50, 15, 100)),
        scorePlayer(0), scoreAi(0) {
    }

    void update()
    {
        float dt = clock.restart().asSeconds();

        // Player input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            m_player.moveY(-PADDLE_SPEED * dt);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            m_player.moveY(PADDLE_SPEED * dt);

        // Simple AI logic (follows the ball slowly)
        RectangleShape aiRect = m_ai.getRect();
        float aiCenter = aiRect.y + aiRect.height / 2;
        float ballY = m_ball.getPosition().y;

        if (ballY < aiCenter - 10)
            m_ai.moveY(-PADDLE_SPEED  * dt);
        else if (ballY > aiCenter + 10)
            m_ai.moveY(PADDLE_SPEED  * dt);

        // Ball logic
        m_ball.update(dt);

        // Collision detection
        RectangleShape r1 = m_player.getRect();
        RectangleShape r2 = m_ai.getRect();
        Vector2d b = m_ball.getPosition();

        // Player collision
        if (b.x - m_ball.getRadius() < r1.x + r1.width &&
            b.x + m_ball.getRadius() > r1.x &&
            b.y > r1.y && b.y < r1.y + r1.height) {
            m_ball.setVelocity(Vector2d(std::abs(m_ball.getVelocity().x), m_ball.getVelocity().y));
        }

        // AI collision
        if (b.x + m_ball.getRadius() > r2.x &&
            b.x - m_ball.getRadius() < r2.x + r2.width &&
            b.y > r2.y && b.y < r2.y + r2.height) {
            m_ball.setVelocity(Vector2d(-std::abs(m_ball.getVelocity().x), m_ball.getVelocity().y));
        }

        // Score handling
        if (b.x < 0) {
            scoreAi++;
            m_ball.setPosition(Vector2d(WIDTH / 2, HEIGHT / 2));
            m_ball.setVelocity(Vector2d(BALL_SPEED, BALL_SPEED));
        }
        if (b.x > WIDTH) {
            scorePlayer++;
            m_ball.setPosition(Vector2d(WIDTH / 2, HEIGHT / 2));
            m_ball.setVelocity(Vector2d(-BALL_SPEED, BALL_SPEED));
        }

        // Game over check
        if (scorePlayer == 10 || scoreAi == 10) {
            winState = GAME_OVER;
            if (scorePlayer == 10)
                player = HUMAN;
            else
                player = AI;
        }
    }

    void render(sf::RenderWindow& window) 
    {
        window.setFramerateLimit(0);
        
        sf::RectangleShape paddleHuman;
        paddleHuman.setSize(sf::Vector2f(m_player.getRect().width, m_player.getRect().height));
        paddleHuman.setPosition(m_player.getRect().x, m_player.getRect().y);
        paddleHuman.setFillColor(sf::Color::White);
        window.draw(paddleHuman);

        sf::RectangleShape paddleAi;
        paddleAi.setSize(sf::Vector2f(m_ai.getRect().width, m_ai.getRect().height));
        paddleAi.setPosition(m_ai.getRect().x, m_ai.getRect().y);
        paddleAi.setFillColor(sf::Color::White);
        window.draw(paddleAi);

        sf::Text text;
        text.setFont(font);
        text.setString("YOU: " + std::to_string(scorePlayer));
        text.setPosition(50, 10);
        window.draw(text);

        text.setString("AI: " + std::to_string(scoreAi));
        text.setPosition(WIDTH - 150, 10);
        window.draw(text);

        sf::CircleShape ballShape;
        ballShape.setRadius(m_ball.getRadius());
        ballShape.setOrigin(m_ball.getRadius(), m_ball.getRadius());
        ballShape.setPosition(m_ball.getPosition().x, m_ball.getPosition().y);
        ballShape.setFillColor(sf::Color::White);
        window.draw(ballShape);
    }
};
