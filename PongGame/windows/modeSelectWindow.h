#pragma once
#include <iostream>
#include <string>
#include "essentials/ball.h"
#include "essentials/paddle.h"
#include "essentials/vars.h"
#include "essentials/utility.h"
#include "gamewindow.h"

class ModeSelectWindow : public GameWindow
{
    int gamemode = 0;
public:
    void handleEvent(const sf::Event& event)
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
            std::cout << "ERROR!" << std::endl;
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
            std::cout << "ERROR!" << std::endl;
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
            std::cout << "ERROR!" << std::endl;
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
            std::cout << "ERROR!" << std::endl;
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