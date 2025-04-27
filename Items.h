#ifndef ITEMS_H
#define ITEMS_H

#include <SDL2/SDL.h>
#include <vector>

class Items {
public:
    enum ItemType { BIG_GOLD, SMALL_GOLD, MYSTERY_BOX, BOMB };

    struct Item {
        ItemType type;
        SDL_Rect rect;
        int points;
        bool isActive;
    };

    struct Explosion {
        SDL_Rect rect;
        double timeRemaining;
    };

Items(int level, SDL_Renderer* renderer);
    ~Items();

    void update();
    void render(SDL_Renderer* renderer);
    std::vector<Item>& getItems();
    void triggerExplosion(int x, int y);

private:
    std::vector<Item> items;
    std::vector<Explosion> explosions;
    SDL_Texture* bigGoldTexture;
    SDL_Texture* smallGoldTexture;
    SDL_Texture* mysteryBoxTexture;
    SDL_Texture* bombTexture;
    SDL_Texture* explosionTexture;
    void initializeItems(int level);
    bool loadTexture(const char* filePath, SDL_Renderer* renderer, SDL_Texture*& texture);
};

#endif // ITEMS_H