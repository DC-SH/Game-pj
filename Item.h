#ifndef ITEM_H
#define ITEM_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

enum ItemType { GOLD, BIG_GOLD, BOMB, MYSTERY_BOX };

class Item {
public:
    Item(SDL_Renderer* renderer, ItemType type, int x, int y);
    ~Item();
    bool loadTexture(const char* filePath, SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);
    SDL_Rect getRect() const;
    ItemType getType() const;
    void setRect(SDL_Rect newRect);

private:
    SDL_Texture* texture;
    SDL_Rect rect;
    ItemType type;
};

#endif // ITEM_H