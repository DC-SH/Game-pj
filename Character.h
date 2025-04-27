#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "Items.h" // Bao gồm Items.h để sử dụng Items::Item và Items*

class Character {
public:
    Character(SDL_Renderer* renderer);
    ~Character();

    bool loadTexture(const char* filePath, SDL_Renderer* renderer);
    void update(int dx);
    void render(SDL_Renderer* renderer);
    SDL_Point getHookPosition();
    void castLine(bool isCasting);
    void checkCollisionWithItems(std::vector<Items::Item>& items, int& score, Items* itemsObj); // Khai báo khớp với định nghĩa
    bool isHookAttached() const;

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
    const int MOVE_AREA_BOTTOM = 300;
    SDL_Point hook;
    bool isLineExtended;
    double lineCurrentLength;
    double lineTargetLength;
    double lineDropSpeed;
    double linePullTime;
    double linePullDuration;
    bool isPullingUp;
    bool hookAttached;
    int attachedItemPoints;
};

#endif // CHARACTER_H