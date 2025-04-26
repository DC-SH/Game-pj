#include "Character.h"
#include <iostream>

Character::Character(SDL_Renderer* renderer) 
    : texture(nullptr), speed(5), rodAngle(0) {
    // Khởi tạo vị trí ban đầu của nhân vật (chill.png)
    rect = { 0, 225, 75, 75 }; // Kích thước 75x75, y = 225 để căn giữa vùng màu cam

    // Khởi tạo cần câu
    rod = { rect.x + rect.w / 2, rect.y - 50, 10, 50 }; // Thanh cần câu: rộng 10, dài 50, gắn phía trên nhân vật
}

Character::~Character() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

bool Character::loadTexture(const char* filePath, SDL_Renderer* renderer) {
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

void Character::update(int dx) {
    // Cập nhật vị trí nhân vật dựa trên đầu vào từ phím
    rect.x += dx * speed; // Di chuyển theo dx (dx = 1: sang phải, dx = -1: sang trái)

    // Kiểm tra va chạm với mép vùng di chuyển
    if (rect.x < MOVE_AREA_LEFT) {
        rect.x = MOVE_AREA_LEFT; // Giữ nhân vật không vượt ra ngoài mép trái
    } else if (rect.x + rect.w > MOVE_AREA_RIGHT) {
        rect.x = MOVE_AREA_RIGHT - rect.w; // Giữ nhân vật không vượt ra ngoài mép phải
    }

    // Đảm bảo nhân vật nằm trong vùng màu cam theo chiều dọc
    if (rect.y < MOVE_AREA_TOP) {
        rect.y = MOVE_AREA_TOP;
    } else if (rect.y + rect.h > MOVE_AREA_BOTTOM) {
        rect.y = MOVE_AREA_BOTTOM - rect.h;
    }

    // Cập nhật vị trí cần câu để di chuyển cùng nhân vật
    rod.x = rect.x + rect.w / 2 - rod.w / 2; // Căn giữa cần câu theo chiều ngang của nhân vật
    rod.y = rect.y - rod.h; // Đặt cần câu phía trên đầu nhân vật
}

void Character::setRodAngle(bool isCasting) {
    // Cập nhật góc xoay của cần câu
    if (isCasting) {
        rodAngle = 150.0; // Khi nhấn Space, xoay 150 độ
    } else {
        rodAngle = 0.0; // Khi thả Space, trở về thẳng đứng
    }
}

void Character::render(SDL_Renderer* renderer) {
    // Vẽ nhân vật
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }

    // Vẽ cần câu
    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Màu nâu cho cần câu
    SDL_Point pivot = { rod.w / 2, rod.h }; // Điểm xoay ở đầu dưới của cần câu
    SDL_RenderCopyEx(renderer, nullptr, nullptr, &rod, rodAngle, &pivot, SDL_FLIP_NONE);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Đặt lại màu mặc định
}