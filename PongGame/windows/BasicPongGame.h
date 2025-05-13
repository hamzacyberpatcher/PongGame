#pragma once
#include <string>
#include "../essentials/ball.h"
#include "../essentials/paddle.h"
#include "../essentials/vars.h"
#include "../essentials/utility.h"
#include "./gamewindow.h"

class BasicPongGame : public GameWindow
{
protected:
    Ball m_ball;
    Paddle m_p1;
    Paddle m_p2;
    int scoreP1;
    int scoreP2;
    bool gameStartPause;
    bool gamePause;
    bool m_pausePressedLastFrame = false;
public:
    BasicPongGame()
        : m_ball(Vector2d(WIDTH / 2, HEIGHT / 2), 15),
        m_p1(RectangleShape(50, HEIGHT / 2 - 50, 15, 100)),
        m_p2(RectangleShape(WIDTH - 65, HEIGHT / 2 - 50, 15, 100)), scoreP1(0), scoreP2(0),
        gameStartPause(true), gamePause(false) {
    }

    void handleEvent(const sf::Event& event)
    {
        if (gameStartPause)
        {
            for (int k = sf::Keyboard::A; k < sf::Keyboard::KeyCount; k++)
            {
                if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(k)))
                    gameStartPause = false;
            }
        }

        bool isPausePressedNow = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);

        if (isPausePressedNow && !m_pausePressedLastFrame) {
            gamePause = !gamePause;
        }

        m_pausePressedLastFrame = isPausePressedNow;
    }

    void moveP1(float dt)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            m_p1.moveY(-PADDLE_SPEED * dt);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            m_p1.moveY(PADDLE_SPEED * dt);
    }

    virtual void moveP2(float dt) = 0;

    virtual void paddleCollision()
    {
        // Collision with paddles
        RectangleShape r1 = m_p1.getRect();
        RectangleShape r2 = m_p2.getRect();
        Vector2d b = m_ball.getPosition();

        // Simple AABB collision
        if (b.x - m_ball.getRadius() < r1.x + r1.width &&
            b.x + m_ball.getRadius() > r1.x &&
            b.y > r1.y && b.y < r1.y + r1.height)
        {

            m_ball.setVelocity(Vector2d(std::abs(m_ball.getVelocity().x), m_ball.getVelocity().y));
        }

        if (b.x + m_ball.getRadius() > r2.x &&
            b.x - m_ball.getRadius() < r2.x + r2.width &&
            b.y > r2.y && b.y < r2.y + r2.height)
        {
            m_ball.setVelocity(Vector2d(-std::abs(m_ball.getVelocity().x), m_ball.getVelocity().y));
        }

        // Reset ball if out of bounds
        if (b.x < 0) {
            scoreP2++;
            m_ball.setPosition(Vector2d(WIDTH / 2, HEIGHT / 2));
            m_ball.setVelocity(Vector2d(BALL_SPEED, BALL_SPEED));
        }
        if (b.x > WIDTH) {
            scoreP1++;
            m_ball.setPosition(Vector2d(WIDTH / 2, HEIGHT / 2));
            m_ball.setVelocity(Vector2d(-BALL_SPEED, BALL_SPEED));
        }
    }

    virtual void gameOverCheck()
    {
        if (scoreP1 == 10 || scoreP2 == 10)
        {
            winState = GAME_OVER;
            if (winState == PLAY_GAME_PLAYERS)
            {
                player = (scoreP1 == 10) ? PLAYER_1 : PLAYER_2;
            }
            else
            {
                player = (scoreP1 == 10) ? HUMAN : AI;
            }
        }
    }

    virtual void update(float dt = 0) {
        
        if (!gameStartPause && !gamePause)
        {

            // Input

            moveP1(dt);
            moveP2(dt);

            // Ball logic
            m_ball.update(dt);

            paddleCollision();

            gameOverCheck();
        }
    }

    virtual void render(sf::RenderWindow& window) {
        window.setFramerateLimit(0);

        if (gameStartPause || gamePause)
        {
            sf::Text text;
            text.setFont(font);
            text.setString(gameStartPause ? "Press Any key to start playing" : "Press Escape to continue playing");
            text.setCharacterSize(30);
            text.setPosition(WIDTH / 2 - 200, HEIGHT / 2 - 60);
            window.draw(text);
        }

        // Draw paddles
        sf::RectangleShape paddle1;
        paddle1.setSize(sf::Vector2f(m_p1.getRect().width, m_p1.getRect().height));
        paddle1.setPosition(m_p1.getRect().x, m_p1.getRect().y);
        paddle1.setFillColor(sf::Color::White);
        window.draw(paddle1);

        sf::RectangleShape paddle2;
        paddle2.setSize(sf::Vector2f(m_p2.getRect().width, m_p2.getRect().height));
        paddle2.setPosition(m_p2.getRect().x, m_p2.getRect().y);
        paddle2.setFillColor(sf::Color::White);
        window.draw(paddle2);



        sf::Text text;
        text.setFont(font);
        text.setString((winState == PLAY_GAME_PLAYERS ? "PLAYER 1: " : "HUMAN: ") + std::to_string(scoreP1));
        text.setPosition(80, 10);

        window.draw(text);

        text.setString((winState == PLAY_GAME_PLAYERS ? "PLAYER 2: " : "AI: ") + std::to_string(scoreP2));
        text.setPosition(WIDTH - 255, 10);

        window.draw(text);


        // Draw ball
        sf::CircleShape ballShape;
        ballShape.setRadius(m_ball.getRadius());
        ballShape.setOrigin(m_ball.getRadius(), m_ball.getRadius());
        ballShape.setPosition(m_ball.getPosition().x, m_ball.getPosition().y);
        ballShape.setFillColor(sf::Color::White);
        window.draw(ballShape);
    }
};