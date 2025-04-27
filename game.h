#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "Character.h"
#include "Items.h"

class Game {
public:
    Game();
    ~Game();
    bool init();
    void run();

private:
    enum GameState { MENU, PLAYING };
    void loadResources();
    void processEvents();
    void update(int mouseX, int mouseY);
    void render();
    void cleanup();

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* bgTexture;
    SDL_Texture* bgGameTexture;
    SDL_Texture* buttonStartTexture;
    SDL_Texture* buttonPlayTexture;
    SDL_Rect buttonRect;
    bool isButtonPlay;
    bool isRunning;
    bool isPaused;
    GameState currentState;
    Character* character;
    Items* items;
    int characterDx;
    bool isCasting;
    int score;
    TTF_Font* font;
    SDL_Texture* scoreTexture;
    SDL_Rect scoreRect;
    void updateScoreTexture();
};

#endif // GAME_H