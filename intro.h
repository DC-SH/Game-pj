#ifndef INTRO_H
#define INTRO_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class Intro {
private:
    SDL_Texture* startTexture; // "background 1.1.png"
    SDL_Texture* playTexture;  // "background 1.2.png"
    SDL_Texture* pointerTexture; // "pointer.png"
    SDL_Rect startButtonRect;
    bool isPlayScreen;
    SDL_Renderer* renderer;
    int mouseX, mouseY;

public:
    Intro(SDL_Renderer* renderer);
    ~Intro();
    bool handleEvents(SDL_Event& e);
    void render();
    bool isGameStarted() const { return isPlayScreen; }
};

#endif