#include "end.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

using namespace std;

End::End(SDL_Renderer* renderer, TTF_Font* font, bool win) : renderer(renderer), font(font), win(win) {
}

End::~End() {
}

void End::render() {
    SDL_RenderClear(renderer);

    if (!font) {
        cerr << "Font is nullptr, cannot render text!" << endl;
        SDL_RenderPresent(renderer);
        return;
    }

    const char* message = win ? "YOU WIN!" : "YOU LOSE!";
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, message, { 255, 255, 255 });
    if (!textSurface) {
        cerr << "TTF_RenderText_Solid failed: " << TTF_GetError() << endl;
        SDL_RenderPresent(renderer);
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << endl;
        SDL_FreeSurface(textSurface);
        SDL_RenderPresent(renderer);
        return;
    }

    SDL_Rect textRect = { (800 - textSurface->w) / 2, (600 - textSurface->h) / 2, textSurface->w, textSurface->h }; // Chính giữa
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    SDL_RenderPresent(renderer);
}