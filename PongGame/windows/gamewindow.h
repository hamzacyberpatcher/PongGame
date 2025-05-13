#pragma once

// Game class
class GameWindow
{
public:
    virtual void update(float dt = 0) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual void handleEvent(const sf::Event& event) {}
};