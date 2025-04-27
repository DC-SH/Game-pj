#include "Items.h"
#include <SDL2/SDL_image.h> // Thêm header này
#include <cstdlib>
#include <ctime>
#include <iostream>

Items::Items(int level, SDL_Renderer* renderer) 
    : bigGoldTexture(nullptr), smallGoldTexture(nullptr), mysteryBoxTexture(nullptr), 
      bombTexture(nullptr), explosionTexture(nullptr) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    loadTexture("biggold.png", renderer, bigGoldTexture);
    loadTexture("minigold.png", renderer, smallGoldTexture);
    loadTexture("mysbox.png", renderer, mysteryBoxTexture);
    loadTexture("bomb.png", renderer, bombTexture);
    loadTexture("explosion.png", renderer, explosionTexture);

    initializeItems(level);
}

Items::~Items() {
    if (bigGoldTexture) SDL_DestroyTexture(bigGoldTexture);
    if (smallGoldTexture) SDL_DestroyTexture(smallGoldTexture);
    if (mysteryBoxTexture) SDL_DestroyTexture(mysteryBoxTexture);
    if (bombTexture) SDL_DestroyTexture(bombTexture);
    if (explosionTexture) SDL_DestroyTexture(explosionTexture);
    items.clear();
    explosions.clear();
}

bool Items::loadTexture(const char* filePath, SDL_Renderer* renderer, SDL_Texture*& texture) {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    SDL_Surface* surface = IMG_Load(filePath);
    if (!surface) {
        std::cerr << "Không thể tải " << filePath << ": " << IMG_GetError() << std::endl;
        return false;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        std::cerr << "Không thể tạo texture cho " << filePath << ": " << IMG_GetError() << std::endl;
        return false;
    }
    return true;
}

void Items::initializeItems(int level) {
    items.clear();

    if (level == 1) {
        for (int i = 0; i < 2; i++) {
            Item bigGold;
            bigGold.type = BIG_GOLD;
            bigGold.rect = { std::rand() % 350 + 800, std::rand() % 300 + 300, 50, 50 };
            bigGold.points = 20;
            bigGold.isActive = true;
            items.push_back(bigGold);
        }

        for (int i = 0; i < 5; i++) {
            Item smallGold;
            smallGold.type = SMALL_GOLD;
            smallGold.rect = { std::rand() % 370 + 800, std::rand() % 300 + 300, 30, 30 };
            smallGold.points = 10;
            smallGold.isActive = true;
            items.push_back(smallGold);
        }

        for (int i = 0; i < 1; i++) {
            Item mysteryBox;
            mysteryBox.type = MYSTERY_BOX;
            mysteryBox.rect = { std::rand() % 360 + 800, std::rand() % 300 + 300, 40, 40 };
            mysteryBox.points = 50;
            mysteryBox.isActive = true;
            items.push_back(mysteryBox);
        }

        for (int i = 0; i < 3; i++) {
            Item bomb;
            bomb.type = BOMB;
            bomb.rect = { std::rand() % 370 + 800, std::rand() % 300 + 300, 30, 30 };
            bomb.points = -30;
            bomb.isActive = true;
            items.push_back(bomb);
        }
    }
}

void Items::triggerExplosion(int x, int y) {
    Explosion explosion;
    explosion.rect = { x, y, 30, 30 };
    explosion.timeRemaining = 0.5;
    explosions.push_back(explosion);
}

void Items::update() {
    for (auto it = explosions.begin(); it != explosions.end();) {
        it->timeRemaining -= 1.0 / 60.0;
        if (it->timeRemaining <= 0) {
            it = explosions.erase(it);
        } else {
            ++it;
        }
    }
}

void Items::render(SDL_Renderer* renderer) {
    for (const auto& item : items) {
        if (item.isActive) {
            SDL_Texture* texture = nullptr;
            switch (item.type) {
                case BIG_GOLD:
                    texture = bigGoldTexture;
                    break;
                case SMALL_GOLD:
                    texture = smallGoldTexture;
                    break;
                case MYSTERY_BOX:
                    texture = mysteryBoxTexture;
                    break;
                case BOMB:
                    texture = bombTexture;
                    break;
            }
            if (texture) {
                SDL_RenderCopy(renderer, texture, nullptr, &item.rect);
            }
        }
    }

    for (const auto& explosion : explosions) {
        if (explosionTexture) {
            SDL_RenderCopy(renderer, explosionTexture, nullptr, &explosion.rect);
        }
    }
}

std::vector<Items::Item>& Items::getItems() {
    return items;
}