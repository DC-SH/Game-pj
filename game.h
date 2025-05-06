#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
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
    enum GameState { MENU, PLAYING, GAME_OVER };
    void loadResources();
    void processEvents();
    void update(int mouseX, int mouseY);
    void render();
    void cleanup();
    void generateItems();
    void renderText(const std::string& text, int x, int y, SDL_Color color, bool centered = false);

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
    
    // Biến cho điểm số, mục tiêu, thời gian
    int score;
    int aim;
    float timeLeft;
    bool hasWon;
    TTF_Font* font;
    bool showRestartText;
    float blinkTimer;

    // Biến cho hiệu ứng nổ
    SDL_Texture* explosionTexture;
    bool isExploding;
    int explosionFrame;
    float explosionTimer;
    const int EXPLOSION_FRAMES = 7;
    const float EXPLOSION_FRAME_TIME = 0.0333f; // Giảm 3 lần (0.1 / 3)
    SDL_Point explosionPosition; // Lưu vị trí bom khi nổ
};

#endif // GAME_H