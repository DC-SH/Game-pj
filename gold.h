#ifndef GOLD_H
#define GOLD_H

#include <SDL2/SDL.h>

struct Gold {
    SDL_Rect rect;
    int value;
    bool isBomb;
    bool isMysteryBox;
    bool isPickedUp;
};

#endif