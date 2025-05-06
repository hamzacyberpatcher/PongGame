#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

const int WIDTH = 1600;
const int HEIGHT = 900;
const float PADDLE_SPEED = 400;
const float BALL_SPEED = 500;

sf::Font font;

// Utility structs
struct Vector2d {
    float x, y;
    Vector2d(float x = 0, float y = 0) : x(x), y(y) {}
};

struct RectangleShape {
    float x, y, width, height;
    RectangleShape(float x = 0, float y = 0, float width = 0, float height = 0)
        : x(x), y(y), width(width), height(height) {
    }
};

// Game elements
class Ball {
    Vector2d m_position;
    Vector2d m_velocity;
    float m_radius;

public:
    Ball(Vector2d pos, float radius)
        : m_position(pos), m_velocity(BALL_SPEED, BALL_SPEED), m_radius(radius) {
    }

    Vector2d getPosition() const { return m_position; }
    Vector2d getVelocity() const { return m_velocity; }
    float getRadius() const { return m_radius; }

    void setPosition(Vector2d pos) { m_position = pos; }
    void setVelocity(Vector2d vel) { m_velocity = vel; }

    void update(float dt) {
        m_position.x += m_velocity.x * dt;
        m_position.y += m_velocity.y * dt;

        // Bounce off top and bottom
        if (m_position.y - m_radius < 0 || m_position.y + m_radius > HEIGHT)
            m_velocity.y = -m_velocity.y;
    }
};

class Paddle {
    RectangleShape m_rect;

public:
    Paddle(RectangleShape rect) : m_rect(rect) {}

    RectangleShape getRect() const { return m_rect; }
    void setRect(RectangleShape rect) { m_rect = rect; }

    void moveY(float dy) {
        m_rect.y += dy;
        // Clamp to screen
        if (m_rect.y < 0) m_rect.y = 0;
        if (m_rect.y + m_rect.height > HEIGHT) m_rect.y = HEIGHT - m_rect.height;
    }
};

// Game class
class PongGame {
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
    }

    void render(sf::RenderWindow& window) {
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
        text.setString("PLAYER 1: " + to_string(scoreP1));
        text.setPosition(50, 10);

        window.draw(text);

        text.setString("PLAYER 2: " + to_string(scoreP2));
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

// Main loop
int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Pong Game");
    window.setFramerateLimit(60);
    font.loadFromFile("./fonts/font.ttf");
    PongGame game;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        game.update();

        window.clear(sf::Color::Black);
        game.render(window);
        window.display();
    }

    return 0;
}
