#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include "Character.h"
#include "Item.h"

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
    void generateItems();

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
    int characterDx;
    bool isCasting;
    std::vector<Item*> items;
};

#endif // GAME_H