#include "player.h"
#include "game.h"
#include <cmath>
#include <iostream>
#include <algorithm>

using namespace std;

Player::Player(int x, int y, SDL_Renderer* renderer) : baseX(x), baseY(y), angle(0.0f), swingSpeed(0.05f), maxSwingAngle(75.0f), pickupSpeed(5), lastPickupTime(0), renderer(renderer) {
    maxSwingRadian = maxSwingAngle * M_PI / 180.0f;
    pickupDuration = 1000 / pickupSpeed;
    characterTexture = IMG_LoadTexture(renderer, "chill gay.png");
    if (!characterTexture) {
        cerr << "Failed to load character texture! IMG_Error: " << IMG_GetError() << endl;
    }
}

Player::~Player() {
    SDL_DestroyTexture(characterTexture);
}

void Player::updateSwing() {
    angle += swingSpeed;
    if (angle > maxSwingRadian || angle < -maxSwingRadian) {
        swingSpeed = -swingSpeed; // Reverse direction
    }
}

void Player::attemptPickup(vector<Gold>& golds, int& score, bool& gameOver, int level, int& timeLeft) {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastPickupTime < pickupDuration) return;

    lastPickupTime = currentTime;
    SDL_Rect hookRect = { baseX - 25, baseY - 25, 50, 50 }; // Hook's hitbox

    for (auto& gold : golds) {
        if (!gold.isPickedUp && SDL_HasIntersection(&gold.rect, &hookRect)) {
            gold.isPickedUp = true;
            if (gold.isBomb) {
                // Handle bomb effects based on level
                if (level == 1) {
                    timeLeft -= 10;
                    score = max(0, score - 100);
                    // Remove 1 random gold
                    auto it = find_if(golds.begin(), golds.end(), [](const Gold& g) { return !g.isPickedUp && !g.isBomb && !g.isMysteryBox; });
                    if (it != golds.end()) {
                        it->isPickedUp = true;
                    } else {
                        // If no gold, remove a mystery box
                        it = find_if(golds.begin(), golds.end(), [](const Gold& g) { return !g.isPickedUp && g.isMysteryBox; });
                        if (it != golds.end()) {
                            it->isPickedUp = true;
                        } else {
                            gameOver = true; // Lose if no items left
                        }
                    }
                } else if (level == 2) {
                    timeLeft -= 15;
                    score = max(0, score - 150);
                    // Remove 2 random golds
                    for (int i = 0; i < 2; ++i) {
                        auto it = find_if(golds.begin(), golds.end(), [](const Gold& g) { return !g.isPickedUp && !g.isBomb && !g.isMysteryBox; });
                        if (it != golds.end()) {
                            it->isPickedUp = true;
                        } else {
                            it = find_if(golds.begin(), golds.end(), [](const Gold& g) { return !g.isPickedUp && g.isMysteryBox; });
                            if (it != golds.end()) {
                                it->isPickedUp = true;
                            } else {
                                gameOver = true;
                            }
                        }
                    }
                } else if (level == 3) {
                    timeLeft -= 20;
                    score = max(0, score - 200);
                    // Remove 2 big golds, replace with 1 bomb and 1 mystery box
                    for (int i = 0; i < 2; ++i) {
                        auto it = find_if(golds.begin(), golds.end(), [](const Gold& g) { return !g.isPickedUp && !g.isBomb && !g.isMysteryBox && g.value == 175; });
                        if (it != golds.end()) {
                            it->isPickedUp = true;
                        } else {
                            it = find_if(golds.begin(), golds.end(), [](const Gold& g) { return !g.isPickedUp && !g.isBomb && !g.isMysteryBox && g.value == 100; });
                            if (it != golds.end()) {
                                it->isPickedUp = true;
                            } else {
                                it = find_if(golds.begin(), golds.end(), [](const Gold& g) { return !g.isPickedUp && g.isMysteryBox; });
                                if (it != golds.end()) {
                                    it->isPickedUp = true;
                                } else {
                                    gameOver = true;
                                }
                            }
                        }
                    }
                    // Add 1 bomb and 1 mystery box
                    Gold newBomb;
                    newBomb.rect = { rand() % 600 + 100, rand() % 200 + 300, 50, 50 };
                    newBomb.isBomb = true;
                    newBomb.isPickedUp = false;
                    newBomb.isMysteryBox = false;
                    golds.push_back(newBomb);

                    Gold newMysteryBox;
                    newMysteryBox.rect = { rand() % 600 + 100, rand() % 200 + 300, 50, 50 };
                    newMysteryBox.isBomb = false;
                    newMysteryBox.isPickedUp = false;
                    newMysteryBox.isMysteryBox = true;
                    golds.push_back(newMysteryBox);
                }
                gameOver = true; // End game if a bomb is hit
            } else if (gold.isMysteryBox) {
                // Handle mystery box effects based on level
                int effect = rand() % 3;
                if (level == 1) {
                    if (effect == 0) timeLeft += 12.5;
                    else if (effect == 1) score = max(0, score - (rand() % 66 + 85));
                    else { timeLeft -= 12.5; score += (rand() % 66 + 85); }
                } else if (level == 2) {
                    if (effect == 0) timeLeft += 17.5;
                    else if (effect == 1) score = max(0, score - (rand() % 66 + 120));
                    else { timeLeft -= 17.5; score += (rand() % 66 + 120); }
                } else if (level == 3) {
                    if (effect == 0) timeLeft += 22.5;
                    else if (effect == 1) score = max(0, score - (rand() % 66 + 155));
                    else { timeLeft -= 22.5; score += (rand() % 66 + 155); }
                }
            } else {
                score += gold.value;
            }
        }
    }
}

void Player::render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, baseX, 100, baseX, baseY);
    SDL_Rect characterRect = { baseX - 25, 50, 50, 50 };
    SDL_RenderCopy(renderer, characterTexture, NULL, &characterRect);
}

