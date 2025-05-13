#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Item.h"

class Character {
public:
    Character(SDL_Renderer* renderer);
    ~Character();

    void update(int dx);
    void render(SDL_Renderer* renderer);
    void castLine(bool isCasting);
    SDL_Point getHookPosition();
    bool hasCaughtItem() const;
    void catchItem();
    void resetCatch();
    void setCaughtItem(Item* item);
    Item* getCaughtItem() const;
    bool isItemCaught() const;
    void reset(); // Added to reset character state

private:
    bool loadTexture(const char* filePath, SDL_Renderer* renderer);
    SDL_Texture* texture;
    SDL_Rect rect;
    SDL_Rect rod;
    SDL_Point hook;
    int speed;
    double rodAngle;
    double angleSpeed;
    bool angleDirection;
    bool isLineExtended;
    double lineCurrentLength;
    double lineTargetLength;
    double lineDropSpeed;
    double linePullTime;
    double linePullDuration;
    bool isPullingUp;
    bool hasCaughtItemFlag;
    Item* caughtItem;
    bool isCaught;
    int currentFrame;
    bool isMoving;
    float animationTimer;

    const int MOVE_AREA_LEFT = 0;
    const int MOVE_AREA_RIGHT = 1200;
    const int MOVE_AREA_TOP = 0;
    const int MOVE_AREA_BOTTOM = 600;
    const float FRAME_TIME = 0.1f;
    const double BASE_PULL_SPEED = 300.0;
};

#endif // CHARACTER_H