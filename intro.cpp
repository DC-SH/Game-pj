#include "intro.h"
#include <iostream>

using namespace std;

Intro::Intro(SDL_Renderer* renderer) : renderer(renderer), isPlayScreen(false), mouseX(0), mouseY(0) {
    startTexture = IMG_LoadTexture(renderer, "background 1.1.png");
    playTexture = IMG_LoadTexture(renderer, "background 1.2.png");
    pointerTexture = IMG_LoadTexture(renderer, "pointer.png");
    if (!startTexture || !playTexture || !pointerTexture) {
        cerr << "Failed to load intro textures! IMG_Error: " << IMG_GetError() << endl;
    }
    startButtonRect = { 350, 400, 100, 50 };
}

Intro::~Intro() {
    SDL_DestroyTexture(startTexture);
    SDL_DestroyTexture(playTexture);
    SDL_DestroyTexture(pointerTexture);
}

bool Intro::handleEvents(SDL_Event& e) {
    if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) {
        SDL_GetMouseState(&mouseX, &mouseY);
        bool inside = (mouseX >= startButtonRect.x && mouseX <= startButtonRect.x + startButtonRect.w &&
                       mouseY >= startButtonRect.y && mouseY <= startButtonRect.y + startButtonRect.h);

        if (inside) {
            isPlayScreen = true; // Switch to "background 1.2.png" immediately
        }
        if (inside && e.type == SDL_MOUSEBUTTONDOWN) {
            return true; // Start the game
        }
    }
    return false;
}

void Intro::render() {
    if (isPlayScreen) {
        SDL_RenderCopy(renderer, playTexture, NULL, NULL);
    } else {
        SDL_RenderCopy(renderer, startTexture, NULL, NULL);
    }
    SDL_Rect pointerRect = { mouseX, mouseY, 20, 20 };
    SDL_RenderCopy(renderer, pointerTexture, NULL, &pointerRect);
}