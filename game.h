#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <string>
#include "Character.h"
#include "Item.h"

class Game {
public:
    Game();
    ~Game();

    bool init();
    void run();
    void loadResources();
    void loadSounds();
    void generateItems();
    void processEvents();
    void update(int mouseX, int mouseY);
    void render();
    void renderText(const std::string& text, int x, int y, SDL_Color color, bool centered = false);
    void cleanup();

private:
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
    enum GameState { MENU, PLAYING, GAME_OVER };
    GameState currentState;
    Character* character;
    int characterDx;
    bool isCasting;
    std::vector<Item*> items;
    int score;
    float timeLeft;
    int aim;
    bool hasWon;
    TTF_Font* font;
    bool showRestartText;
    float blinkTimer;
    SDL_Texture* explosionTexture;
    bool isExploding;
    int explosionFrame;
    float explosionTimer;
    SDL_Point explosionPosition;
    const float EXPLOSION_FRAME_TIME = 0.0583f; // ~233ms for 4 frames
    const int EXPLOSION_FRAMES = 4;

    // Variables for restart countdown
    bool isRestarting;
    float restartTimer;

    // Sound effects
    Mix_Chunk* winSound;
    Mix_Chunk* loseSound;
    Mix_Chunk* bombSound;
    Mix_Chunk* goldSound;
    Mix_Chunk* spaceClickSound;
    int movementSoundChannel; // Channel for looping movement sound
};

#endif // GAME_H