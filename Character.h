#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Item.h"

class Character {
public:
    Character(SDL_Renderer* renderer);
    ~Character();

    bool loadTexture(const char* filePath, SDL_Renderer* renderer);
    void update(int dx);
    void render(SDL_Renderer* renderer);
    SDL_Point getHookPosition();
    void castLine(bool isCasting);
    bool hasCaughtItem() const;
    void catchItem();
    void resetCatch();
    void setCaughtItem(Item* item);
    Item* getCaughtItem() const;
    bool isItemCaught() const;

private:
    SDL_Texture* texture;
    SDL_Rect rect;
    SDL_Rect rod;
    double rodAngle;
    double angleSpeed;
    bool angleDirection;
    int speed;
    const int MOVE_AREA_LEFT = 0;
    const int MOVE_AREA_RIGHT = 1200;
    const int MOVE_AREA_TOP = 200;
    const int MOVE_AREA_BOTTOM = 310; // Tăng từ 300 lên 310
    SDL_Point hook;
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
};

#endif // CHARACTER_H