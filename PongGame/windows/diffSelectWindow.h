#pragma once
#include <iostream>
#include <string>
#include "../essentials/vars.h"
#include "../windows/gamewindow.h"

class DifficultySelectWindow : public GameWindow
{
    int gamemode = 0;
public:
    void handleEvent(const sf::Event& event)
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

    void update(float dt = 0)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            gamemode--;
            if (gamemode < 0)
                gamemode = 2;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            gamemode = (gamemode + 1) % 3;
        }

    }

    void render(sf::RenderWindow& window)
    {
        window.setFramerateLimit(FPS);


        sf::Texture easyTexture;
        if (!easyTexture.loadFromFile("sprites/easy.png")) // Replace with your image file path
        {
            // Error handling if the image fails to load
            std::cout << "ERROR!" << std::endl;
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
            std::cout << "ERROR!" << std::endl;
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
            std::cout << "ERROR!" << std::endl;
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
