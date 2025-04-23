#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "gold.h"

class Player {
private:
    int baseX, baseY; // Base position of the hook
    float angle; // Angle for swinging
    float swingSpeed; // Speed of the swing
    float maxSwingAngle; // Maximum swing angle
    float maxSwingRadian; // Maximum swing angle in radians
    int pickupSpeed; // Speed of picking up items
    Uint32 lastPickupTime; // Last time a pickup was attempted
    int pickupDuration; // Duration of the pickup action
    SDL_Texture* characterTexture; // Texture for the character "chill gay"
    SDL_Renderer* renderer;

public:
    Player(int x, int y, SDL_Renderer* renderer);
    ~Player();
    void updateSwing(); // Update the swinging motion of the hook
    void attemptPickup(std::vector<Gold>& golds, int& score, bool& gameOver, int level, int& timeLeft); // Attempt to pick up gold or bombs
    void render(); // Render the hook and character
    int getBaseX() const { return baseX; }
    int getBaseY() const { return baseY; }
};

#endif