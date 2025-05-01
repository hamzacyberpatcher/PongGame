#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;

const int WIDTH = 1600;
const int HEIGHT = 900;

// all of the enums

enum GAME_STATE
{
	MENU,
	IN_GAME
};


enum BUTTON_STATE
{
	UP,
	DOWN,
	HOVER
};

enum MOUSE_STATE
{
	UP,
	DOWN
};

enum PLAY_STATE
{
	SERVE_PLAYER_ONE,
	SERVE_PLAYER_TWO,
	TOWARD_PLAYER_ONE,
	TOWARD_PLAYER_TWO
};

// all of the utility classes

class RectangleShape
{
public:
	float x;
	float y;
	float width;
	float height;
	RectangleShape(float x = 0, float y = 0, float width = 0, float height = 0) : x(x), y(y), width(width), height(height) {}
};

class Vector2d
{
public:
	float x;
	float y;
	Vector2d(float x = 0, float y = 0) : x(x), y(y) {}
};

// all of the core game elements

class Ball
{
	Vector2d m_position;
	float m_radius;
	Vector2d m_velocity;
public:
	Ball(Vector2d startingPosition, float radius)
	{
		m_position = startingPosition;
		m_radius = radius; 
	}

	Vector2d getPositon() { return m_position; }

	float getRadius() { return m_radius; }

	Vector2d getVelocity() { return m_velocity; }

	void setPosition(Vector2d position) { m_position = position; }

	void setVelocity(Vector2d velocity) { m_velocity = velocity; }
};

class Paddle
{
	RectangleShape m_rect;
public:
	Paddle(RectangleShape startingPosition) { m_rect = startingPosition; }
	RectangleShape GetPositionSize() { return m_rect; }
	void SetPositionSize(RectangleShape newPositionAndSize) { m_rect = newPositionAndSize; }
};

class Court
{
	RectangleShape m_dimensions;
public:
	Court(RectangleShape dimensions) : m_dimensions(dimensions) {}
	RectangleShape GetDimensions() { return m_dimensions; }
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Comway's Game of Life", sf::Style::Default);
	window.setFramerateLimit(60);

	sf::RectangleShape rectangle;
	rectangle.setSize(sf::Vector2f(100, 50));
	rectangle.setOutlineColor(sf::Color::Red);
	rectangle.setOutlineThickness(5);
	rectangle.setPosition({ 10, 20 });

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		window.draw(rectangle);

		window.display();
	}

	return 0;

	return 0;
}