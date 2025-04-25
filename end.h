#ifndef END_H
#define END_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class End {
private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    bool win;

public:
    End(SDL_Renderer* renderer, TTF_Font* font, bool win);
    ~End();
    void render();
};

#endif