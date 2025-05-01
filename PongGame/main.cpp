#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;

const int WIDTH = 1600;
const int HEIGHT = 900;

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