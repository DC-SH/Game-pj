#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Character {
public:
    Character(SDL_Renderer* renderer);
    ~Character();

    bool loadTexture(const char* filePath, SDL_Renderer* renderer); // Tải texture
    void update(int dx); // Cập nhật vị trí nhân vật, cần câu và dây
    void render(SDL_Renderer* renderer); // Vẽ nhân vật, cần câu, mũi tên và dây
    SDL_Point getHookPosition(); // Lấy vị trí đầu dây câu
    void castLine(bool isCasting); // Thả dây khi nhấn Space

private:
    SDL_Texture* texture; // Texture của nhân vật (chill.png)
    SDL_Rect rect; // Vị trí và kích thước của nhân vật
    SDL_Rect rod; // Hình chữ nhật đại diện cho cần câu (chỉ dùng để lưu chiều dài, không dùng để vẽ)
    double rodAngle; // Góc xoay của cần câu (-75 đến 75 độ)
    double angleSpeed; // Tốc độ xoay của cần câu
    bool angleDirection; // Hướng xoay (true: tăng, false: giảm)
    int speed; // Tốc độ di chuyển
    const int MOVE_AREA_LEFT = 0; // Giới hạn trái của vùng di chuyển
    const int MOVE_AREA_RIGHT = 1200; // Giới hạn phải của vùng di chuyển
    const int MOVE_AREA_TOP = 200; // Giới hạn trên của vùng di chuyển
    const int MOVE_AREA_BOTTOM = 300; // Giới hạn dưới của vùng di chuyển
    SDL_Point hook; // Vị trí đầu dây câu
    bool isLineExtended; // Trạng thái dây câu (đang thả hay không)
    double lineCurrentLength; // Độ dài hiện tại của dây
    double lineTargetLength; // Độ dài mục tiêu của dây (khi thả xuống)
    double lineDropSpeed; // Tốc độ thả dây (px/s)
    double linePullTime; // Thời gian hiện tại của quá trình kéo dây lên
    double linePullDuration; // Thời gian kéo dây lên (phụ thuộc vào số điểm cục vàng)
    bool isPullingUp; // Trạng thái đang kéo dây lên
};

#endif // CHARACTER_H