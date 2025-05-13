#pragma once
#include <string>
#include "../essentials/vars.h"
#include "../windows/gamewindow.h"

class GameOverWindow : public GameWindow
{
public:
    void handleEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
            winState = MAIN_GAME;
            player = NONE;
        }
    }
    void update(float dt = 0)
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
            std::cout << "ERROR!" << std::endl;
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
            std::cout << "ERROR!" << std::endl;
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
            std::cout << "ERROR!" << std::endl;
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
            std::cout << "ERROR!" << std::endl;
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