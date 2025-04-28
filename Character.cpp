#include "Character.h"
#include <iostream>
#include <cmath>
#include <vector>

Character::Character(SDL_Renderer* renderer)
    : texture(nullptr), speed(1), rodAngle(0), angleSpeed(1.0), angleDirection(true),
      isLineExtended(false), lineCurrentLength(0), lineTargetLength(0), lineDropSpeed(200),
      linePullTime(0), linePullDuration(0), isPullingUp(false) {
    // Khởi tạo vị trí ban đầu của nhân vật (chill.png)
    rect = { 0, 225, 75, 75 }; // Kích thước 75x75, y = 225 để căn giữa vùng màu cam

    // Khởi tạo cần câu (chỉ dùng để lưu chiều dài, không dùng để vẽ hình chữ nhật)
    rod = { rect.x + rect.w / 2, rect.y + rect.h / 2, 20, 50 }; // Chiều dài 50px
    hook = { rect.x + rect.w / 2, rect.y + rect.h / 2 + 50 }; // Đầu dây câu ban đầu ở cuối cần câu
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
    // Cập nhật vị trí nhân vật dựa trên đầu vào từ phím (chỉ khi không thả dây)
    if (!isLineExtended) {
        rect.x += dx * speed;

        // Kiểm tra va chạm với mép vùng di chuyển
        if (rect.x < MOVE_AREA_LEFT) {
            rect.x = MOVE_AREA_LEFT;
        } else if (rect.x + rect.w > MOVE_AREA_RIGHT) {
            rect.x = MOVE_AREA_RIGHT - rect.w;
        }
    }

    // Đảm bảo nhân vật nằm trong vùng màu cam theo chiều dọc
    if (rect.y < MOVE_AREA_TOP) {
        rect.y = MOVE_AREA_TOP;
    } else if (rect.y + rect.h > MOVE_AREA_BOTTOM) {
        rect.y = MOVE_AREA_BOTTOM - rect.h;
    }

    // Cập nhật vị trí cần câu để di chuyển cùng nhân vật
    rod.x = rect.x + rect.w / 2; // Điểm đầu của đoạn thẳng (trung tâm nhân vật)
    rod.y = rect.y + rect.h / 2;

    // Cập nhật góc xoay tự động (chỉ khi không thả dây)
    if (!isLineExtended) {
        if (angleDirection) {
            rodAngle += angleSpeed;
            if (rodAngle >= 75.0) {
                rodAngle = 75.0;
                angleDirection = false;
            }
        } else {
            rodAngle -= angleSpeed;
            if (rodAngle <= -75.0) {
                rodAngle = -75.0;
                angleDirection = true;
            }
        }
    }

    // Tính vị trí đầu dưới cần câu (đầu có mũi tên)
    double rad = rodAngle * M_PI / 180.0;
    int rodEndX = rod.x + static_cast<int>(rod.h * sin(rad));
    int rodEndY = rod.y + static_cast<int>(rod.h * cos(rad));

    // Cập nhật vị trí đầu dây câu (hook)
    if (isLineExtended && !isPullingUp) {
        // Thả dây xuống theo hướng của cần câu với vận tốc cố định
        lineCurrentLength += lineDropSpeed * (1.0 / 60.0); // 60 FPS, mỗi khung hình tăng 1/60 giây
        hook.x = rodEndX + static_cast<int>((lineCurrentLength - rod.h) * sin(rad));
        hook.y = rodEndY + static_cast<int>((lineCurrentLength - rod.h) * cos(rad));

        // Giới hạn để dây không vượt quá màn hình
        if (hook.y >= 600) {
            hook.y = 600;
            lineCurrentLength = sqrt(pow(hook.x - rod.x, 2) + pow(hook.y - rod.y, 2));
            isPullingUp = true; // Bắt đầu kéo lên khi chạm đáy (giả lập nhặt được vàng)
        }
    } else if (isPullingUp) {
        // Kéo dây lên với tốc độ phụ thuộc vào số điểm
        linePullTime += 1.0 / 60.0; // Tăng thời gian kéo
        if (linePullTime > linePullDuration) {
            linePullTime = linePullDuration;
        }

        // Tính độ dài dây hiện tại khi kéo lên
        double remainingLength = lineCurrentLength * (1.0 - (linePullTime / linePullDuration));
        if (remainingLength < rod.h) remainingLength = rod.h; // Đảm bảo dây không ngắn hơn chiều dài cần câu

        hook.x = rodEndX + static_cast<int>((remainingLength - rod.h) * sin(rad));
        hook.y = rodEndY + static_cast<int>((remainingLength - rod.h) * cos(rad));

        // Khi dây kéo về vị trí ban đầu, reset trạng thái
        if (remainingLength <= rod.h) {
            isLineExtended = false;
            isPullingUp = false;
            lineCurrentLength = rod.h;
            linePullTime = 0;
            hook.x = rodEndX;
            hook.y = rodEndY;
        }
    } else {
        // Dây chưa thả, đặt ở đầu dưới cần câu
        hook.x = rodEndX;
        hook.y = rodEndY;
        lineCurrentLength = rod.h;
        linePullTime = 0;
        isPullingUp = false;
    }
}

void Character::castLine(bool isCasting) {
    // Thả dây khi nhấn Space
    if (isCasting && !isLineExtended) {
        isLineExtended = true;
        lineCurrentLength = rod.h; // Bắt đầu từ đầu mũi tên
        linePullTime = 0;
        isPullingUp = false;

        // Giả lập số điểm của cục vàng (chưa cập nhật)
        int goldPoints = 10; // Thay bằng số điểm thực tế khi cập nhật cục vàng
        linePullDuration = goldPoints * 0.1; // Mỗi điểm tương ứng 0.1 giây
        if (linePullDuration < 0.5) {
            linePullDuration = 0.5; // Đảm bảo thời gian tối thiểu là 0.5 giây
        }
    } else if (!isCasting && isLineExtended && !isPullingUp) {
        // Bắt đầu kéo lên nếu đang thả dây mà thả phím Space
        isPullingUp = true;
    }
}

SDL_Point Character::getHookPosition() {
    return hook; // Trả về vị trí đầu dây câu
}

void Character::render(SDL_Renderer* renderer) {
    // Vẽ nhân vật
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }

    // Vẽ cần câu (dưới dạng đoạn thẳng)
    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Màu nâu
    double rad = rodAngle * M_PI / 180.0;
    int pivotX = rod.x; // Điểm đầu đoạn thẳng (trung tâm nhân vật)
    int pivotY = rod.y;

    // Tính vị trí đầu dưới cần câu (đầu có mũi tên)
    int rodEndX = rod.x + static_cast<int>(rod.h * sin(rad));
    int rodEndY = rod.y + static_cast<int>(rod.h * cos(rad));

    // Vẽ đoạn thẳng từ pivot đến rodEnd
    SDL_RenderDrawLine(renderer, pivotX, pivotY, rodEndX, rodEndY);

    // Vẽ mũi tên ở đầu dưới cần câu (đối xứng qua trục cần câu)
    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Màu nâu (giữ cùng màu với cần câu)
    SDL_Point arrow[3];
    arrow[0] = { rodEndX, rodEndY }; // Đỉnh mũi tên
    double arrowAngle = rad; // Góc của mũi tên trùng với góc cần câu
    arrow[1] = { rodEndX + static_cast<int>(10 * sin(arrowAngle - 0.3)), 
                 rodEndY + static_cast<int>(10 * cos(arrowAngle - 0.3)) }; // Cạnh trái
    arrow[2] = { rodEndX + static_cast<int>(10 * sin(arrowAngle + 0.3)), 
                 rodEndY + static_cast<int>(10 * cos(arrowAngle + 0.3)) }; // Cạnh phải
    SDL_RenderDrawLines(renderer, arrow, 3);
    SDL_RenderDrawLine(renderer, arrow[2].x, arrow[2].y, arrow[1].x, arrow[1].y); // Đóng tam giác

    // Vẽ dây câu (xuất phát từ đỉnh mũi tên: arrow[0])
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Màu đen cho dây
    SDL_RenderDrawLine(renderer, arrow[0].x, arrow[0].y, hook.x, hook.y);

    // Vẽ hình parabol úp ngược (giống mỏ neo) tại vị trí đầu dây
    if (isLineExtended) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Màu đen cho mỏ neo
        std::vector<SDL_Point> parabolaPoints;
        int width = 20; // Chiều rộng
        int height = 10; // Chiều cao
        int steps = 20; // Số điểm để vẽ parabol

        // Tính các điểm trên đường parabol úp ngược: y = a(x - h)^2 + k
        // Đỉnh parabol tại (hook.x, hook.y), mở xuống dưới
        for (int i = 0; i <= steps; i++) {
            double x = hook.x - width / 2 + (width * i / steps); // x từ hook.x - width/2 đến hook.x + width/2
            double a = height / (width * width / 4.0); // Hệ số a để chiều cao parabol = height
            double y = hook.y + a * (x - hook.x) * (x - hook.x); // Phương trình parabol
            parabolaPoints.push_back({ static_cast<int>(x), static_cast<int>(y) });
        }

        // Vẽ parabol bằng cách nối các điểm
        SDL_RenderDrawLines(renderer, parabolaPoints.data(), parabolaPoints.size());
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Đặt lại màu mặc định
}