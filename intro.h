#ifndef INTRO_H
#define INTRO_H

#include <SDL2/SDL.h>

class Intro {
private:
    SDL_Renderer* renderer;
    SDL_Texture* introTexture;
    bool gameStarted;

public:
    Intro(SDL_Renderer* renderer);
    ~Intro();
    void handleEvents(SDL_Event& e);
    bool isGameStarted() const;
    void render();
};

#endif