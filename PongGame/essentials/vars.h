#pragma once

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
