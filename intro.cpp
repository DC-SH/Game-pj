#include "intro.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

using namespace std;

Intro::Intro(SDL_Renderer* renderer) : renderer(renderer), gameStarted(false) {
    introTexture = IMG_LoadTexture(renderer, "background 1.1.png"); // Sửa tên
    if (!introTexture) {
        cerr << "Failed to load background 1.1.png: " << IMG_GetError() << endl;
    }
}

Intro::~Intro() {
    SDL_DestroyTexture(introTexture);
}

void Intro::handleEvents(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
        gameStarted = true;
    }
}

bool Intro::isGameStarted() const {
    return gameStarted;
}

void Intro::render() {
    SDL_RenderClear(renderer);
    if (introTexture) {
        SDL_RenderCopy(renderer, introTexture, NULL, NULL);
    } else {
        cerr << "introTexture is nullptr, cannot render!" << endl;
    }
    SDL_RenderPresent(renderer);
}