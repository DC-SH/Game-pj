#ifndef END_H
#define END_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class End {
private:
    SDL_Renderer* renderer;
    SDL_Texture* endTexture; // "background 3.png"
    TTF_Font* font;
    bool won;

public:
    End(SDL_Renderer* renderer, TTF_Font* font, bool won);
    ~End();
    void render();
};

#endif