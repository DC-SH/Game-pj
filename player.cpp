#include "player.h"
#include "game.h"
#include <cmath>
#include <iostream>
#include <algorithm>

using namespace std;

Player::Player(int x, int y, SDL_Renderer* renderer) : baseX(x), baseY(y), angle(0.0f), swingSpeed(0.05f), maxSwingAngle(150.0f), pickupSpeed(5), lastPickupTime(0), renderer(renderer) {
    maxSwingRadian = maxSwingAngle * M_PI / 180.0f;
    pickupDuration = 1000 / pickupSpeed;
    characterTexture = IMG_LoadTexture(renderer, "chill gay.png");
    if (!characterTexture) {
        cerr << "Failed to load character texture (chill gay.png)! IMG_Error: " << IMG_GetError() << endl;
    }
}

Player::~Player() {
    SDL_DestroyTexture(characterTexture);
}

void Player::updateSwing() {
    angle += swingSpeed;
    if (angle > maxSwingRadian || angle < -maxSwingRadian) {
        swingSpeed = -swingSpeed;
    }
}

void Player::attemptPickup(vector<Gold>& golds, int& score, bool& gameOver, int level) {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastPickupTime < pickupDuration) return;

    lastPickupTime = currentTime;
    SDL_Rect hookRect = { baseX - 25, baseY - 25, 50, 50 };

    for (Gold& gold : golds) {
        if (!gold.isPickedUp && SDL_HasIntersection(&gold.rect, &hookRect)) {
            gold.isPickedUp = true;
            if (gold.isBomb) {
                if (level == 1) {
                    score = max(0, score - 100);
                } else if (level == 2) {
                    score = max(0, score - 150);
                } else if (level == 3) {
                    score = max(0, score - 200);
                }
                gameOver = true;
            } else if (gold.isMysteryBox) {
                int effect = rand() % 3;
                if (level == 1) {
                    if (effect == 0) score += 50;
                    else if (effect == 1) score = max(0, score - (rand() % 66 + 85));
                    else score += (rand() % 66 + 85);
                } else if (level == 2) {
                    if (effect == 0) score += 50;
                    else if (effect == 1) score = max(0, score - (rand() % 66 + 120));
                    else score += (rand() % 66 + 120);
                } else if (level == 3) {
                    if (effect == 0) score += 50;
                    else if (effect == 1) score = max(0, score - (rand() % 66 + 155));
                    else score += (rand() % 66 + 155);
                }
            } else {
                score += gold.value;
            }
            break; // Chỉ nhặt 1 vật mỗi lần nhấn Space
        }
    }
}

void Player::render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, baseX, 100, baseX, baseY);
    SDL_Rect characterRect = { baseX - 25, 50, 50, 50 };
    if (characterTexture) {
        SDL_RenderCopy(renderer, characterTexture, NULL, &characterRect);
    } else {
        cerr << "characterTexture is nullptr, cannot render!" << endl;
    }
}