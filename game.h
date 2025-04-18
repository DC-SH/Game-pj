#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>

struct Gold {
    SDL_Rect rect;
    bool isBomb;
    bool isPickedUp;
    bool isMysteryBox;
    int value;
};

class Game {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* bigGoldTexture;
    SDL_Texture* miniGoldTexture;
    SDL_Texture* bombTexture;
    SDL_Texture* mysteryBoxTexture;
    SDL_Texture* explosionTexture;
    SDL_Texture* backgroundTexture; // "background 2.png"
    TTF_Font* font;
    std::vector<Gold> golds;
    int score;
    int boundaryY;
    Uint32 startTime;
    int elapsedTime;
    int currentRound;
    int roundTimes[3];
    int roundScores[3];
    bool gameOver;
    int timeLeft;

public:
    Game();
    ~Game();
    bool init();
    void spawnItems(int level);
    void update();
    void render();
    int getScore() const { return score; }
    bool isGameOver() const { return gameOver; }
    void setGameOver(bool over) { gameOver = over; }
    std::vector<Gold>& getGolds() { return golds; }
    SDL_Renderer* getRenderer() const { return renderer; }
    int getCurrentRound() const { return currentRound; }
    int getTimeLeft() const { return timeLeft; }
    void setTimeLeft(int time) { timeLeft = time; }
};

#endif