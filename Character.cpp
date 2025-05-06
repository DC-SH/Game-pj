#include "Character.h"
#include <iostream>
#include <cmath>
#include <vector>

Character::Character(SDL_Renderer* renderer)
    : texture(nullptr), speed(1), rodAngle(0), angleSpeed(1.0), angleDirection(true),
      isLineExtended(false), lineCurrentLength(0), lineTargetLength(0), lineDropSpeed(300),
      linePullTime(0), linePullDuration(0), isPullingUp(false), hasCaughtItemFlag(false),
      caughtItem(nullptr), isCaught(false) {
    rect = { 0, 225, 75, 75 };
    rod = { rect.x + rect.w / 2, rect.y + rect.h / 2, 20, 40 };
    hook = { rect.x + rect.w / 2, rect.y + rect.h / 2 + 40 };
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
    if (!isLineExtended) {
        rect.x += dx * speed;
        if (rect.x < MOVE_AREA_LEFT) {
            rect.x = MOVE_AREA_LEFT;
        } else if (rect.x + rect.w > MOVE_AREA_RIGHT) {
            rect.x = MOVE_AREA_RIGHT - rect.w;
        }
    }

    if (rect.y < MOVE_AREA_TOP) {
        rect.y = MOVE_AREA_TOP;
    } else if (rect.y + rect.h > MOVE_AREA_BOTTOM) {
        rect.y = MOVE_AREA_BOTTOM - rect.h;
    }

    rod.x = rect.x + rect.w / 2;
    rod.y = rect.y + rect.h / 2;

    if (!isLineExtended) {
        if (angleDirection) {
            rodAngle += angleSpeed;
            if (rodAngle >= 45.0) {
                rodAngle = 45.0;
                angleDirection = false;
            }
        } else {
            rodAngle -= angleSpeed;
            if (rodAngle <= -45.0) {
                rodAngle = -45.0;
                angleDirection = true;
            }
        }
    }

    double rad = rodAngle * M_PI / 180.0;
    int rodEndX = rod.x + static_cast<int>(rod.h * sin(rad));
    int rodEndY = rod.y + static_cast<int>(rod.h * cos(rad));

    // Điều chỉnh tốc độ kéo dựa trên vật phẩm
    double pullSpeed = BASE_PULL_SPEED;
    if (caughtItem) {
        ItemType type = caughtItem->getType();
        if (type == BIG_GOLD) {
            pullSpeed /= 2.5; // Chậm gấp 2.5 lần
        } else if (type == GOLD) {
            pullSpeed /= 1.5; // Chậm gấp 1.5 lần
        }
    }

    if (isLineExtended && !isPullingUp) {
        lineCurrentLength += lineDropSpeed * (1.0 / 60.0);
        hook.x = rodEndX + static_cast<int>((lineCurrentLength - rod.h) * sin(rad));
        hook.y = rodEndY + static_cast<int>((lineCurrentLength - rod.h) * cos(rad));

        if (hook.y >= 600) {
            hook.y = 600;
            lineCurrentLength = sqrt(pow(hook.x - rod.x, 2) + pow(hook.y - rod.y, 2));
            isPullingUp = true;
        }
    } else if (isPullingUp) {
        linePullTime += 1.0 / 60.0;
        if (linePullTime > linePullDuration) {
            linePullTime = linePullDuration;
        }

        double remainingLength = lineCurrentLength * (1.0 - (linePullTime / linePullDuration));
        if (remainingLength < rod.h) remainingLength = rod.h;

        hook.x = rodEndX + static_cast<int>((remainingLength - rod.h) * sin(rad));
        hook.y = rodEndY + static_cast<int>((remainingLength - rod.h) * cos(rad));

        if (caughtItem) {
            SDL_Rect itemRect = caughtItem->getRect();
            itemRect.x = hook.x - itemRect.w / 2;
            itemRect.y = hook.y - itemRect.h / 2;
            caughtItem->setRect(itemRect);
        }

        if (remainingLength <= rod.h) {
            isLineExtended = false;
            isPullingUp = false;
            lineCurrentLength = rod.h;
            linePullTime = 0;
            hook.x = rodEndX;
            hook.y = rodEndY;
            resetCatch();
            caughtItem = nullptr;
            isCaught = false;
        }
    } else {
        hook.x = rodEndX;
        hook.y = rodEndY;
        lineCurrentLength = rod.h;
        linePullTime = 0;
        isPullingUp = false;
    }
}

void Character::castLine(bool isCasting) {
    double rad = rodAngle * M_PI / 180.0;
    int rodEndX = rod.x + static_cast<int>(rod.h * sin(rad));
    int rodEndY = rod.y + static_cast<int>(rod.h * cos(rad));

    if (isCasting && !isLineExtended && !isPullingUp && rodEndY <= MOVE_AREA_BOTTOM) {
        isLineExtended = true;
        lineCurrentLength = rod.h;
        linePullTime = 0;
        isPullingUp = false;

        int itemPoints = 10;
        linePullDuration = itemPoints * 0.1 * (2.0 / 3.0);
        if (linePullDuration < 0.5 * (2.0 / 3.0)) {
            linePullDuration = 0.5 * (2.0 / 3.0);
        }
    } else if (!isCasting && isLineExtended && !isPullingUp) {
        isPullingUp = true;
    }
}

SDL_Point Character::getHookPosition() {
    return hook;
}

bool Character::hasCaughtItem() const {
    return hasCaughtItemFlag;
}

void Character::catchItem() {
    hasCaughtItemFlag = true;
}

void Character::resetCatch() {
    hasCaughtItemFlag = false;
}

void Character::setCaughtItem(Item* item) {
    caughtItem = item;
    isCaught = true;
    isPullingUp = true;
}

Item* Character::getCaughtItem() const {
    return caughtItem;
}

bool Character::isItemCaught() const {
    return isCaught;
}

void Character::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }

    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
    double rad = rodAngle * M_PI / 180.0;
    int pivotX = rod.x;
    int pivotY = rod.y;

    int rodEndX = rod.x + static_cast<int>(rod.h * sin(rad));
    int rodEndY = rod.y + static_cast<int>(rod.h * cos(rad));

    for (int offset = -3; offset <= 2; ++offset) {
        int x1 = pivotX + offset;
        int y1 = pivotY;
        int x2 = rodEndX + offset;
        int y2 = rodEndY;
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }

    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
    SDL_Point arrow[3];
    arrow[0] = { rodEndX, rodEndY };
    double arrowAngle = rad;
    arrow[1] = { rodEndX + static_cast<int>(10 * sin(arrowAngle - 0.3)),
                 rodEndY + static_cast<int>(10 * cos(arrowAngle - 0.3)) };
    arrow[2] = { rodEndX + static_cast<int>(10 * sin(arrowAngle + 0.3)),
                 rodEndY + static_cast<int>(10 * cos(arrowAngle + 0.3)) };
    SDL_RenderDrawLines(renderer, arrow, 3);
    SDL_RenderDrawLine(renderer, arrow[2].x, arrow[2].y, arrow[1].x, arrow[1].y);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer, arrow[0].x, arrow[0].y, hook.x, hook.y);

    if (isLineExtended) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        if (!isCaught) {
            std::vector<SDL_Point> parabolaPoints;
            int width = 10;
            int height = 5;
            int steps = 20;

            for (int i = 0; i <= steps; i++) {
                double x = hook.x - width / 2 + (width * i / steps);
                double a = height / (width * width / 4.0);
                double y = hook.y + a * (x - hook.x) * (x - hook.x);
                parabolaPoints.push_back({ static_cast<int>(x), static_cast<int>(y) });
            }
            SDL_RenderDrawLines(renderer, parabolaPoints.data(), parabolaPoints.size());
        } else {
            int radius = 10;
            SDL_Point center = { hook.x, hook.y };
            std::vector<SDL_Point> circlePoints;
            int steps = 50;

            for (int i = 0; i <= steps; i++) {
                double angle = 2.0 * M_PI * i / steps;
                int x = center.x + static_cast<int>(radius * cos(angle));
                int y = center.y + static_cast<int>(radius * sin(angle));
                circlePoints.push_back({ x, y });
            }
            SDL_RenderDrawLines(renderer, circlePoints.data(), circlePoints.size());
        }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}