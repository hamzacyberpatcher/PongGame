#pragma once
#include <string>
#include "essentials/ball.h"
#include "essentials/paddle.h"
#include "essentials/vars.h"
#include "essentials/utility.h"
#include "windows/gamewindow.h"


class PongGameAiHard : public GameWindow {
    Ball m_ball;
    Paddle m_player;
    Paddle m_ai;
    sf::Clock clock;
    float aiVelocity = 0.0f;
    float aiReactionTimer = 0.0f;
    const float AI_MAX_SPEED = PADDLE_SPEED;
    const float AI_ACCEL = 800.0f;
    const float AI_REACTION_DELAY = 0.08f; // 80 ms reaction delay

    int scorePlayer;
    int scoreAi;

public:
    PongGameAiHard()
        : m_ball(Vector2d(WIDTH / 2, HEIGHT / 2), 15),
        m_player(RectangleShape(50, HEIGHT / 2 - 50, 15, 100)),
        m_ai(RectangleShape(WIDTH - 65, HEIGHT / 2 - 50, 15, 100)),
        scorePlayer(0), scoreAi(0) {
    }

    float simulateWallBounces(Vector2d pos, Vector2d vel, float targetX) {
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

    void update() override {
        float dt = clock.restart().asSeconds();
        aiReactionTimer += dt;

        // Player movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            m_player.moveY(-PADDLE_SPEED * dt);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            m_player.moveY(PADDLE_SPEED * dt);

        // Smart AI movement
        RectangleShape aiRect = m_ai.getRect();
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

            m_ai.moveY(aiVelocity * dt);
        }

        // Ball update
        m_ball.update(dt);

        // Paddle collisions
        RectangleShape r1 = m_player.getRect();
        RectangleShape r2 = m_ai.getRect();
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
            scoreAi++;
            resetBall(-BALL_SPEED);
        }
        if (b.x > WIDTH) {
            scorePlayer++;
            resetBall(BALL_SPEED);
        }

        // Game over
        if (scorePlayer == 10 || scoreAi == 10) {
            winState = GAME_OVER;
            player = (scorePlayer == 10) ? HUMAN : AI;
        }
    }

    void resetBall(float vx) {
        m_ball.setPosition(Vector2d(WIDTH / 2, HEIGHT / 2));
        m_ball.setVelocity(Vector2d(vx, BALL_SPEED));
        aiVelocity = 0.0f;
        aiReactionTimer = 0.0f;
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