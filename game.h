#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include "gold.h"

class Game {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* bigGoldTexture;
    SDL_Texture* miniGoldTexture;
    SDL_Texture* bombTexture;
    SDL_Texture* mysteryBoxTexture;
    SDL_Texture* explosionTexture;
    SDL_Texture* backgroundTexture;
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
    int maxTime;
    // Thêm các biến để quản lý texture cho score và time
    SDL_Texture* scoreTexture;
    SDL_Texture* timeTexture;
    SDL_Rect scoreRect;
    SDL_Rect timeRect;
    int lastScore; // Giá trị score trước đó
    int lastTimeLeft; // Giá trị timeLeft trước đó

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
    void setTimeLeft(int time) {
        timeLeft = time;
        if (timeLeft > maxTime) timeLeft = maxTime;
    }
    bool allItemsPickedUp() const {
        for (const Gold& gold : golds) {
            if (!gold.isPickedUp) return false;
        }
        return true;
    }
    // Thêm hàm để cập nhật texture
    void updateScoreTexture();
    void updateTimeTexture();
};

#endif