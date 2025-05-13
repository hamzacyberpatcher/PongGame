#pragma once
#include <string>
#include "essentials/ball.h"
#include "essentials/paddle.h"
#include "essentials/vars.h"
#include "essentials/utility.h"
#include "windows/gamewindow.h"

class PongGameAiMid : public GameWindow {
    Ball m_ball;
    Paddle m_player;
    Paddle m_ai;
    sf::Clock clock;
    int scorePlayer;
    int scoreAi;

public:
    PongGameAiMid()
        : m_ball(Vector2d(WIDTH / 2, HEIGHT / 2), 15),
        m_player(RectangleShape(50, HEIGHT / 2 - 50, 15, 100)),
        m_ai(RectangleShape(WIDTH - 65, HEIGHT / 2 - 50, 15, 100)),
        scorePlayer(0), scoreAi(0) {
    }

    void update() override {
        float dt = clock.restart().asSeconds();

        // Player input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            m_player.moveY(-PADDLE_SPEED * dt);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            m_player.moveY(PADDLE_SPEED * dt);

        // --- Smart AI logic ---
        RectangleShape aiRect = m_ai.getRect();
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
                m_ai.moveY(-PADDLE_SPEED * dt);
            else if (predictedY > aiCenter + 10)
                m_ai.moveY(PADDLE_SPEED * dt);
        }

        // Ball update
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

        // Scoring
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
            player = (scorePlayer == 10) ? HUMAN : AI;
        }
    }

    void render(sf::RenderWindow& window) override {
        window.setFramerateLimit(0);
        auto drawRect = [&](RectangleShape r) {
            sf::RectangleShape shape;
            shape.setSize(sf::Vector2f(r.width, r.height));
            shape.setPosition(r.x, r.y);
            shape.setFillColor(sf::Color::White);
            window.draw(shape);
            };

        drawRect(m_player.getRect());
        drawRect(m_ai.getRect());

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