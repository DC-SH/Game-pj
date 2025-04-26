#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Character {
public:
    Character(SDL_Renderer* renderer);
    ~Character();

    bool loadTexture(const char* filePath, SDL_Renderer* renderer); // Tải texture
    void update(int dx); // Cập nhật vị trí nhân vật dựa trên đầu vào từ phím
    void render(SDL_Renderer* renderer); // Vẽ nhân vật và cần câu
    void setRodAngle(bool isCasting); // Cập nhật góc xoay của cần câu

private:
    SDL_Texture* texture; // Texture của nhân vật (chill.png)
    SDL_Rect rect; // Vị trí và kích thước của nhân vật
    SDL_Rect rod; // Hình chữ nhật đại diện cho cần câu
    double rodAngle; // Góc xoay của cần câu (0 độ: thẳng đứng, 150 độ: khi nhấn Space)
    int speed; // Tốc độ di chuyển
    const int MOVE_AREA_LEFT = 0; // Giới hạn trái của vùng di chuyển
    const int MOVE_AREA_RIGHT = 1200; // Giới hạn phải của vùng di chuyển
    const int MOVE_AREA_TOP = 200; // Giới hạn trên của vùng di chuyển (vùng màu cam)
    const int MOVE_AREA_BOTTOM = 300; // Giới hạn dưới của vùng di chuyển (vùng màu cam)
};

#endif // CHARACTER_H