#include "end.h"

using namespace std;

// End::End(SDL_Renderer* renderer, TTF_Font* font, bool won) : renderer(renderer), font(font), won(won) {
//     endTexture = IMG_LoadTexture(renderer, "background 3.png");
// }

End::End(SDL_Renderer* renderer, TTF_Font* font, bool won) : renderer(renderer), font(font), won(won) {
    SDL_Surface* surface = IMG_Load("background 3.png"); // Tải hình ảnh
    if (surface) {
        endTexture = SDL_CreateTextureFromSurface(renderer, surface); // Chuyển đổi surface thành texture
        SDL_FreeSurface(surface); // Giải phóng surface sau khi sử dụng
    } else {
        endTexture = nullptr; // Xử lý nếu không tải được hình ảnh
    }
}

End::~End() {
    SDL_DestroyTexture(endTexture);
}

void End::render() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, endTexture, NULL, NULL);

    string message = won ? "You Win!" : "You Lose!";
    SDL_Color textColor = { 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, message.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect renderQuad = { 350, 300, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    SDL_RenderPresent(renderer);
    SDL_Delay(2000);
}