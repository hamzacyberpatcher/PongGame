#pragma once
#pragma once
#include <string>
#include "essentials/ball.h"
#include "essentials/paddle.h"
#include "essentials/vars.h"
#include "essentials/utility.h"

class GameWindow
{
public:
    virtual void update() = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual void handleEvent(const sf::Event& event) {}
};

// Game class
class PongGame : public GameWindow {
    Ball m_ball;
    Paddle m_p1;
    Paddle m_p2;
    sf::Clock clock;
    int scoreP1;
    int scoreP2;

public:
    PongGame()
        : m_ball(Vector2d(WIDTH / 2, HEIGHT / 2), 15),
        m_p1(RectangleShape(50, HEIGHT / 2 - 50, 15, 100)),
        m_p2(RectangleShape(WIDTH - 65, HEIGHT / 2 - 50, 15, 100)), scoreP1(0), scoreP2(0) {
    }

    void update() {
        float dt = clock.restart().asSeconds();

        // Input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            m_p1.moveY(-PADDLE_SPEED * dt);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            m_p1.moveY(PADDLE_SPEED * dt);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            m_p2.moveY(-PADDLE_SPEED * dt);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            m_p2.moveY(PADDLE_SPEED * dt);

        // Ball logic
        m_ball.update(dt);

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

        if (scoreP1 == 10 || scoreP2 == 10)
        {
            winState = GAME_OVER;
            if (scoreP1 == 10)
                player = PLAYER_1;
            else
                player = PLAYER_2;
        }
    }

    void render(sf::RenderWindow& window) {
        window.setFramerateLimit(0);

        // Draw paddles
        auto drawRect = [&](RectangleShape r) {
            sf::RectangleShape shape;
            shape.setSize(sf::Vector2f(r.width, r.height));
            shape.setPosition(r.x, r.y);
            shape.setFillColor(sf::Color::White);
            window.draw(shape);
            };

        drawRect(m_p1.getRect());
        drawRect(m_p2.getRect());



        sf::Text text;
        text.setFont(font);
        text.setString("PLAYER 1: " + std::to_string(scoreP1));
        text.setPosition(50, 10);

        window.draw(text);

        text.setString("PLAYER 2: " + std::to_string(scoreP2));
        text.setPosition(WIDTH - 200, 10);

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