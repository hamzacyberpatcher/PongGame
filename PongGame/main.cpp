#include <SFML/Graphics.hpp>
#include "windows/mainwindow.h"
#include "windows/modeSelectWindow.h"
#include "windows/diffSelectWindow.h"
#include "windows/PongGameHuman.h"
#include "windows/gameOverWindow.h"
#include "AI/easyAI.h"
#include "AI/midAI.h"
#include "AI/hardAI.h"

void setGameWindow(GameWindow*& game)
{
    if (game)
        delete game;
    if (winState == MAIN_GAME)
        game = new MainWindow();
    else if (winState == MODE_SELECT)
        game = new ModeSelectWindow();
    else if (winState == PLAY_GAME_PLAYERS)
        game = new PongGameHuman();
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