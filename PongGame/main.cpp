#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

const int WIDTH = 1600;
const int HEIGHT = 900;
const float PADDLE_SPEED = 400;
const float BALL_SPEED = 500;
const int FPS = 10;

sf::Font font;

enum WINDOW_STATE
{
    MAIN_GAME,
    MODE_SELECT,
    AI_DIFFICULTY_SELECT,
    PLAY_GAME_PLAYERS,
    PLAY_GAME_AI_EASY,
    PLAY_GAME_AI_MID,
    PLAY_GAME_AI_HARD,
    GAME_OVER
};

enum PlayerWon
{
    PLAYER_1,
    PLAYER_2,
    HUMAN,
    AI,
    NONE
};

WINDOW_STATE winState = MAIN_GAME;
PlayerWon player;

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

    void update() override {
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
        text.setString("YOU: " + to_string(scorePlayer));
        text.setPosition(50, 10);
        window.draw(text);

        text.setString("AI: " + to_string(scoreAi));
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
        text.setString("YOU: " + to_string(scorePlayer));
        text.setPosition(50, 10);
        window.draw(text);

        text.setString("AI: " + to_string(scoreAi));
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
        text.setString("YOU: " + to_string(scorePlayer));
        text.setPosition(50, 10);
        window.draw(text);

        text.setString("AI: " + to_string(scoreAi));
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



class MainWindow : public GameWindow
{
public:
    void handleEvent(const sf::Event& event) override {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
            winState = MODE_SELECT;
        }
    }

    void update()
    {
        return;
    }
    void render(sf::RenderWindow& window)
    {
        window.setFramerateLimit(FPS);

        sf::Texture backgroundTexture;
        if (!backgroundTexture.loadFromFile("sprites/background.png")) // Replace with your image file path
        {
            // Error handling if the image fails to load
            cout << "ERROR!" << endl;
        }

        sf::Sprite backgroundSprite;
        backgroundSprite.setTexture(backgroundTexture);

        sf::Vector2u backgroundTextureSize = backgroundTexture.getSize();

        // Center the scaled sprite
        backgroundSprite.setOrigin(backgroundTextureSize.x / 2.f, backgroundTextureSize.y / 2.f);
        backgroundSprite.setPosition(WIDTH / 2.f, HEIGHT / 2.f);

        backgroundSprite.setScale(1400.f / backgroundTextureSize.x, 1000.f / backgroundTextureSize.y);



        sf::Texture swordTexture;

        // Load the image file into the texture
        if (!swordTexture.loadFromFile("sprites/swords.png")) // Replace with your image file path
        {
            // Error handling if the image fails to load
            cout << "ERROR!" << endl;
        }

        sf::Sprite swordsSprite;
        swordsSprite.setTexture(swordTexture);

        sf::Vector2u swordTextureSize = swordTexture.getSize();

        // Center the scaled sprite
        swordsSprite.setOrigin(swordTextureSize.x / 2.f, swordTextureSize.y / 2.f);
        swordsSprite.setPosition(WIDTH / 2.05f, HEIGHT / 4.f);

        swordsSprite.setScale(600.f / swordTextureSize.x, 600.f / swordTextureSize.y);


        sf::Texture ballTexture;
        if (!ballTexture.loadFromFile("sprites/pongball.png")) // Replace with your image file path
        {
            // Error handling if the image fails to load
            cout << "ERROR!" << endl;
        }

        sf::Sprite ballSprite1;
        ballSprite1.setTexture(ballTexture);

        sf::Vector2u ballTextureSize = ballTexture.getSize();

        // Center the scaled sprite
        ballSprite1.setOrigin(ballTextureSize.x / 2.f, ballTextureSize.y / 2.f);
        ballSprite1.setPosition(WIDTH / 2.05f, HEIGHT - 775);

        ballSprite1.setScale(275.f / ballTextureSize.x, 275.f / ballTextureSize.y);


        sf::Texture redThrowTexture;
        if (!redThrowTexture.loadFromFile("sprites/redthrow.png")) // Replace with your image file path
        {
            // Error handling if the image fails to load
            cout << "ERROR!" << endl;
        }

        sf::Texture blueThrowTexture;
        if (!blueThrowTexture.loadFromFile("sprites/bluethrow.png")) // Replace with your image file path
        {
            // Error handling if the image fails to load
            cout << "ERROR!" << endl;
        }

        sf::Sprite throwSprite1;
        throwSprite1.setTexture(redThrowTexture);

        throwSprite1.setOrigin(ballTextureSize.x / 2.f, ballTextureSize.y / 2.f);
        throwSprite1.setPosition(WIDTH - 1300, HEIGHT - 500);
        throwSprite1.setScale(225.f / ballTextureSize.x, 225.f / ballTextureSize.y);

        throwSprite1.setRotation(-25);

        sf::Sprite throwSprite2;
        throwSprite2.setTexture(blueThrowTexture);

        throwSprite2.setOrigin(ballTextureSize.x / 2.f, ballTextureSize.y / 2.f);
        throwSprite2.setPosition(WIDTH - 500, HEIGHT - 575);
        throwSprite2.setScale(225.f / ballTextureSize.x, 225.f / ballTextureSize.y);

        throwSprite2.setRotation(150);



        sf::Text titleText;
        titleText.setFont(font);
        titleText.setString("PONG FIGHT");
        titleText.setCharacterSize(75);
        titleText.setPosition(1600.f / 3.f, 900.f / 3.f);

        sf::Text enterKeyText;
        enterKeyText.setFont(font);
        enterKeyText.setString("PRESS ENTER KEY TO START PONGING!!!");
        enterKeyText.setCharacterSize(35);
        enterKeyText.setPosition(400, 500);

        window.clear(sf::Color::Black);

        window.draw(backgroundSprite);
        window.draw(swordsSprite);
        window.draw(ballSprite1);
        window.draw(throwSprite1);
        window.draw(throwSprite2);

        window.draw(titleText);
        window.draw(enterKeyText);
    }
};

class ModeSelectWindow : public GameWindow
{
    int gamemode = 0;
public:
    void handleEvent(const sf::Event& event) override
    {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
        {
            if (gamemode == 0)
                winState = PLAY_GAME_PLAYERS;
            else
                winState = AI_DIFFICULTY_SELECT;
        }
    }

    void update()
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            gamemode--;
            if (gamemode < 0)
                gamemode = 1;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            gamemode = (gamemode + 1) % 2;
        }

    }

    void render(sf::RenderWindow& window)
    {
        window.setFramerateLimit(FPS);

        // Load the image file into the texture
        sf::Texture swordTexture;
        if (!swordTexture.loadFromFile("sprites/swords.png")) // Replace with your image file path
        {
            // Error handling if the image fails to load
            cout << "ERROR!" << endl;
        }

        sf::Sprite swordsSprite;
        swordsSprite.setTexture(swordTexture);

        sf::Vector2u swordTextureSize = swordTexture.getSize();

        // Center the scaled sprite
        swordsSprite.setOrigin(swordTextureSize.x / 2.f, swordTextureSize.y / 2.f);
        swordsSprite.setPosition(WIDTH - 400, HEIGHT / 4.f);

        swordsSprite.setScale(600.f / swordTextureSize.x, 600.f / swordTextureSize.y);

        sf::Texture pvpTexture;
        if (!pvpTexture.loadFromFile("sprites/pvp.png")) // Replace with your image file path
        {
            // Error handling if the image fails to load
            cout << "ERROR!" << endl;
        }

        sf::Sprite pvpSprite;
        pvpSprite.setTexture(pvpTexture);

        sf::Vector2u pvpTextureSize = pvpTexture.getSize();

        // Center the scaled sprite
        pvpSprite.setOrigin(pvpTextureSize.x / 2.f, pvpTextureSize.y / 2.f);
        pvpSprite.setPosition(WIDTH - 400, HEIGHT - 300);

        pvpSprite.setScale(700 / pvpTextureSize.x, 600 / pvpTextureSize.y);

        sf::Texture playerTexture;
        if (!playerTexture.loadFromFile("sprites/player.png")) // Replace with your image file path
        {
            // Error handling if the image fails to load
            cout << "ERROR!" << endl;
        }

        sf::Sprite playerSprite;
        playerSprite.setTexture(playerTexture);

        sf::Vector2u playerTextureSize = playerTexture.getSize();

        // Center the scaled sprite
        playerSprite.setOrigin(playerTextureSize.x / 2.f, playerTextureSize.y / 2.f);
        playerSprite.setPosition(WIDTH - 600, HEIGHT - 300);

        playerSprite.setScale(700 / playerTextureSize.x, 600 / playerTextureSize.y);



        sf::Texture cpuTexture;
        if (!cpuTexture.loadFromFile("sprites/cpu.png")) // Replace with your image file path
        {
            // Error handling if the image fails to load
            cout << "ERROR!" << endl;
        }

        sf::Sprite cpuSprite;
        cpuSprite.setTexture(cpuTexture);

        sf::Vector2u cpuTextureSize = cpuTexture.getSize();

        // Center the scaled sprite
        cpuSprite.setOrigin(cpuTextureSize.x / 2.f, cpuTextureSize.y / 2.f);
        cpuSprite.setPosition(WIDTH - 200, HEIGHT - 300);

        cpuSprite.setScale(500 / cpuTextureSize.x, 400 / cpuTextureSize.y);


        sf::Text modeSelectText;
        modeSelectText.setFont(font);
        modeSelectText.setString("SELECT A MODE TO PLAY");
        modeSelectText.setPosition(100, 100);
        modeSelectText.setCharacterSize(70);

        sf::Text pvpText;
        pvpText.setFont(font);
        pvpText.setString("1. PLAY WITH A FRIEND");
        pvpText.setFillColor(gamemode == 0 ? sf::Color::Green : sf::Color::White);
        pvpText.setPosition(100, 400);
        pvpText.setCharacterSize(50);

        sf::Text aiText;
        aiText.setFont(font);
        aiText.setString("2. PLAY WITH CPU");
        aiText.setFillColor(gamemode == 1 ? sf::Color::Green : sf::Color::White);
        aiText.setPosition(100, 700);
        aiText.setCharacterSize(50);

        window.draw(modeSelectText);
        window.draw(pvpText);
        window.draw(aiText);

        if (gamemode == 0)
        {
            window.draw(pvpSprite);
        }
        else
        {
            window.draw(playerSprite);
            window.draw(cpuSprite);
        }

        window.draw(swordsSprite);
    }
};

class DifficultySelectWindow : public GameWindow
{
    int gamemode = 0;
public:
    void handleEvent(const sf::Event& event) override
    {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
        {
            if (gamemode == 0)
                winState = PLAY_GAME_AI_EASY;
            else if (gamemode == 1)
                winState = PLAY_GAME_AI_MID;
            else
                winState = PLAY_GAME_AI_HARD;
        }
    }

    void update()
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            gamemode--;
            if (gamemode < 0)
                gamemode = 2;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            gamemode++;

            if (gamemode == 3)
                gamemode = 0;
        }

    }

    void render(sf::RenderWindow& window)
    {
        window.setFramerateLimit(FPS);


        sf::Texture easyTexture;
        if (!easyTexture.loadFromFile("sprites/easy.png")) // Replace with your image file path
        {
            // Error handling if the image fails to load
            cout << "ERROR!" << endl;
        }

        sf::Sprite easySprite;
        easySprite.setTexture(easyTexture);

        sf::Vector2u easyTextureSize = easyTexture.getSize();

        // Center the scaled sprite
        easySprite.setOrigin(easyTextureSize.x / 2.f, easyTextureSize.y / 2.f);
        easySprite.setPosition(WIDTH - 400, HEIGHT - 500);

        easySprite.setScale(700 / easyTextureSize.x, 600 / easyTextureSize.y);

        sf::Texture normalTexture;
        if (!normalTexture.loadFromFile("sprites/normal.png")) // Replace with your image file path
        {
            // Error handling if the image fails to load
            cout << "ERROR!" << endl;
        }

        sf::Sprite normalSprite;
        normalSprite.setTexture(normalTexture);

        sf::Vector2u normalTextureSize = normalTexture.getSize();

        // Center the scaled sprite
        normalSprite.setOrigin(normalTextureSize.x / 2.f, normalTextureSize.y / 2.f);
        normalSprite.setPosition(WIDTH - 400, HEIGHT - 500);

        normalSprite.setScale(700 / normalTextureSize.x, 600 / normalTextureSize.y);


        sf::Texture hardTexture;
        if (!hardTexture.loadFromFile("sprites/hard.png")) // Replace with your image file path
        {
            // Error handling if the image fails to load
            cout << "ERROR!" << endl;
        }

        sf::Sprite hardSprite;
        hardSprite.setTexture(hardTexture);

        sf::Vector2u hardTextureSize = hardTexture.getSize();

        // Center the scaled sprite
        hardSprite.setOrigin(hardTextureSize.x / 2.f, hardTextureSize.y / 2.f);
        hardSprite.setPosition(WIDTH - 400, HEIGHT - 500);

        hardSprite.setScale(700 / hardTextureSize.x, 600 / hardTextureSize.y);


        sf::Text modeSelectText;
        modeSelectText.setFont(font);
        modeSelectText.setString("SELECT DIFFICULTY OF AI: ");
        modeSelectText.setPosition(100, 100);

        sf::Text ezText;
        ezText.setFont(font);
        ezText.setString("1. EASY");
        ezText.setFillColor(gamemode == 0 ? sf::Color::Green : sf::Color::White);
        ezText.setPosition(100, 200);

        sf::Text midText;
        midText.setFont(font);
        midText.setString("2. MEDIUM");
        midText.setFillColor(gamemode == 1 ? sf::Color::Green : sf::Color::White);
        midText.setPosition(100, 300);

        sf::Text hardText;
        hardText.setFont(font);
        hardText.setString("3. HARD");
        hardText.setFillColor(gamemode == 2 ? sf::Color::Green : sf::Color::White);
        hardText.setPosition(100, 400);

        window.draw(modeSelectText);
        window.draw(ezText);
        window.draw(midText);
        window.draw(hardText);

        if (gamemode == 0)
            window.draw(easySprite);
        else if (gamemode == 1)
            window.draw(normalSprite);
        else
            window.draw(hardSprite);

    }
};

class GameOverWindow : public GameWindow
{
public:
    void handleEvent(const sf::Event& event) override {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
            winState = MAIN_GAME;
            player = NONE;
        }
    }
    void update()
    {
        return;
    }
    void render(sf::RenderWindow& window)
    {
        window.setFramerateLimit(FPS);

        sf::Texture playerWinTexture;
        // Load the image file into the texture
        if (!playerWinTexture.loadFromFile("sprites/playerwin.png")) // Replace with your image file path
        {
            // Error handling if the image fails to load
            cout << "ERROR!" << endl;
        }

        sf::Sprite playerWinSprite;
        playerWinSprite.setTexture(playerWinTexture);

        sf::Vector2u winTextureSize = playerWinTexture.getSize();

        // Center the scaled sprite
        playerWinSprite.setOrigin(winTextureSize.x / 2.f, winTextureSize.y / 2.f);
        playerWinSprite.setPosition(WIDTH / 2.05f, HEIGHT / 2.f);

        playerWinSprite.setScale(400.f / winTextureSize.x, 400.f / winTextureSize.y);


        sf::Texture redWinTexture;
        // Load the image file into the texture
        if (!redWinTexture.loadFromFile("sprites/redwin.png")) // Replace with your image file path
        {
            // Error handling if the image fails to load
            cout << "ERROR!" << endl;
        }

        sf::Sprite redWinSprite;
        redWinSprite.setTexture(redWinTexture);

        // Center the scaled sprite
        redWinSprite.setOrigin(winTextureSize.x / 2.f, winTextureSize.y / 2.f);
        redWinSprite.setPosition(WIDTH / 2.05f, HEIGHT / 2.f);

        redWinSprite.setScale(400.f / winTextureSize.x, 400.f / winTextureSize.y);




        sf::Texture blueWinTexture;
        if (!blueWinTexture.loadFromFile("sprites/bluewin.png")) // Replace with your image file path
        {
            // Error handling if the image fails to load
            cout << "ERROR!" << endl;
        }

        sf::Sprite blueWinSprite;
        blueWinSprite.setTexture(blueWinTexture);

        // Center the scaled sprite
        blueWinSprite.setOrigin(winTextureSize.x / 2.f, winTextureSize.y / 2.f);
        blueWinSprite.setPosition(WIDTH / 2.05f, HEIGHT / 2.f);

        blueWinSprite.setScale(400.f / winTextureSize.x, 400.f / winTextureSize.y);


        sf::Texture lostTexture;
        if (!lostTexture.loadFromFile("sprites/lost.png")) // Replace with your image file path
        {
            // Error handling if the image fails to load
            cout << "ERROR!" << endl;
        }

        sf::Sprite lostSprite;
        lostSprite.setTexture(lostTexture);

        // Center the scaled sprite
        lostSprite.setOrigin(winTextureSize.x / 2.f, winTextureSize.y / 2.f);
        lostSprite.setPosition(WIDTH / 2.05f, HEIGHT / 2.f);

        lostSprite.setScale(400.f / winTextureSize.x, 400.f / winTextureSize.y);

        sf::Text gameOverText;
        gameOverText.setFont(font);
        gameOverText.setString("GAMEOVER!!!");
        gameOverText.setPosition(800, 450);
        gameOverText.setCharacterSize(100);

        sf::Text playerText;
        playerText.setFont(font);
        playerText.setPosition(350, 100);
        playerText.setCharacterSize(75);

        sf::Text enterKey;
        enterKey.setFont(font);
        enterKey.setString("PRESS ENTER TO CONTINUE");
        enterKey.setPosition(400, 650);
        enterKey.setCharacterSize(50);

        if (player == PLAYER_1)
            playerText.setString("PLAYER 1 HAS WON!!!!");
        else if (player == PLAYER_2)
            playerText.setString("PLAYER 2 HAS WON!!!!");
        else if (player == HUMAN)
            playerText.setString("HUMAN HAS WON!!!!!!");
        else if (player == AI)
            playerText.setString("AI HAS WON!!!!!!!!!!");

        window.draw(enterKey);
        window.draw(playerText);
        //window.draw(gameOverText);
        if (player == PLAYER_1)
            window.draw(redWinSprite);
        else if (player == PLAYER_2)
            window.draw(blueWinSprite);
        else if (player == HUMAN)
            window.draw(playerWinSprite);
        else if (player == AI)
            window.draw(lostSprite);
    }
};


void setGameWindow(GameWindow*& game)
{
    if (game)
        delete game;
    if (winState == MAIN_GAME)
        game = new MainWindow();
    else if (winState == MODE_SELECT)
        game = new ModeSelectWindow();
    else if (winState == PLAY_GAME_PLAYERS)
        game = new PongGame();
    else if (winState == AI_DIFFICULTY_SELECT)
        game = new DifficultySelectWindow();
    else if (winState == PLAY_GAME_AI_EASY)
        game = new PongGameAiEasy();
    else if (winState == PLAY_GAME_AI_MID)
        game = new PongGameAiMid();
    else if (winState == PLAY_GAME_AI_HARD)
        game = new PongGameAiHard();
    else if (winState == GAME_OVER)
        game = new GameOverWindow();
}

// Main loop
int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Pong Game");
    window.setFramerateLimit(FPS);
    font.loadFromFile("./fonts/yoster.ttf");
    GameWindow* game = nullptr;
    setGameWindow(game);
    WINDOW_STATE currState = winState;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();
            else
                game->handleEvent(event);

        if (currState != winState)
        {
            setGameWindow(game);
            currState = winState;
        }

        game->update();

        window.clear(sf::Color::Black);
        game->render(window);
        window.display();



    }

    delete game;



    return 0;
}