#include "Item.h"
#include <iostream>

Item::Item(SDL_Renderer* renderer, ItemType type, int x, int y)
    : texture(nullptr), type(type) {
    switch (type) {
        case BIG_GOLD:
            rect = { x, y, 100, 100 };
            break;
        case GOLD:
            rect = { x, y, 150, 150 };
            break;
        case MYSTERY_BOX:
            rect = { x, y, 40, 40 };
            break;
        case BOMB:
            rect = { x, y, 80, 80 }; // Phóng to gấp 2 lần (từ 40x40)
            break;
        default:
            rect = { x, y, 100, 100 };
            break;
    }

    const char* filePath;
    switch (type) {
        case BIG_GOLD: filePath = "biggold.png"; break;
        case GOLD: filePath = "minigold.png"; break;
        case MYSTERY_BOX: filePath = "mysbox.png"; break;
        case BOMB: filePath = "bomb.png"; break;
        default: filePath = "biggold.png"; break;
    }
    if (!loadTexture(filePath, renderer)) {
        std::cerr << "Không thể tải hình ảnh vật phẩm: " << filePath << std::endl;
    }
}

Item::~Item() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

bool Item::loadTexture(const char* filePath, SDL_Renderer* renderer) {
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

void Item::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }
}

SDL_Rect Item::getRect() const {
    return rect;
}

ItemType Item::getType() const {
    return type;
}

void Item::setRect(SDL_Rect newRect) {
    rect = newRect;
}