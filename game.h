#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "Character.h"

class Game {
public:
    Game();
    ~Game();
    bool init();
    void run();

private:
    enum GameState { MENU, PLAYING }; // Trạng thái game
    void loadResources();
    void processEvents();
    void update(int mouseX, int mouseY);
    void render();
    void cleanup();

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* bgTexture; // back1.png (menu)
    SDL_Texture* bgGameTexture; // back2.png (gameplay)
    SDL_Texture* buttonStartTexture; // b1.png
    SDL_Texture* buttonPlayTexture; // b2.png
    SDL_Rect buttonRect;
    bool isButtonPlay; // True khi chuột hover (hiển thị b2.png)
    bool isRunning;
    bool isPaused;
    GameState currentState; // Trạng thái hiện tại
    Character* character; // Đối tượng nhân vật
    int characterDx; // Thay đổi vị trí x của nhân vật (1: phải, -1: trái, 0: dừng)
    bool isCasting; // Trạng thái nhấn Space (đang thả dây hay không)
};

#endif // GAME_H