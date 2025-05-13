#pragma once
#include <iostream>
#include <string>
#include "essentials/ball.h"
#include "essentials/paddle.h"
#include "essentials/vars.h"
#include "essentials/utility.h""
#include "gamewindow.h"


class MainWindow : public GameWindow
{
public:
    void handleEvent(const sf::Event& event)
    {
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
            std::cout << "ERROR!" << std::endl;
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
            std::cout << "ERROR!" << std::endl;
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
            std::cout << "ERROR!" << std::endl;
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
            std::cout << "ERROR!" << std::endl;
        }

        sf::Texture blueThrowTexture;
        if (!blueThrowTexture.loadFromFile("sprites/bluethrow.png")) // Replace with your image file path
        {
            // Error handling if the image fails to load
            std::cout << "ERROR!" << std::endl;
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