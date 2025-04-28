#include "Game.h"
#include <iostream>

Game::Game()
    : window(nullptr)
    , renderer(nullptr)
    , bgTexture(nullptr)
    , bgGameTexture(nullptr)
    , buttonStartTexture(nullptr)
    , buttonPlayTexture(nullptr)
    , isButtonPlay(false)
    , isRunning(false)
    , isPaused(false)
    , currentState(MENU)
    , character(nullptr)
    , characterDx(0)
    , isCasting(false) {
    buttonRect = { (1200 - 200) / 2, (600 - 100) / 2, 200, 100 }; // Căn giữa
}

Game::~Game() {
    cleanup();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Không thể khởi tạo SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Game Fishing",
                             SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED,
                             1200, 600,
                             SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Không thể tạo cửa sổ: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Không thể tạo renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "Không thể khởi tạo SDL_image: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    // Khởi tạo nhân vật và tải texture chill.png
    character = new Character(renderer);
    if (!character->loadTexture("chill.png", renderer)) {
        std::cerr << "Lỗi: Không tìm thấy file chill.png hoặc file không đúng định dạng!" << std::endl;
        delete character;
        character = nullptr;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    loadResources();
    isRunning = true;
    return true;
}

void Game::loadResources() {
    // Tải background 1 (menu)
    SDL_Surface* surface = IMG_Load("back1.png");
    if (!surface) {
        std::cerr << "Không thể tải back1.png: " << IMG_GetError() << std::endl;
        isRunning = false;
        return;
    }
    bgTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!bgTexture) {
        std::cerr << "Không thể tạo texture cho back1.png: " << IMG_GetError() << std::endl;
        isRunning = false;
        return;
    }

    // Tải background 2 (gameplay)
    surface = IMG_Load("back2.png");
    if (!surface) {
        std::cerr << "Không thể tải back2.png: " << IMG_GetError() << std::endl;
        isRunning = false;
        return;
    }
    bgGameTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!bgGameTexture) {
        std::cerr << "Không thể tạo texture cho back2.png: " << IMG_GetError() << std::endl;
        isRunning = false;
        return;
    }

    // Tải nút start (b1.png)
    surface = IMG_Load("b1.png");
    if (!surface) {
        std::cerr << "Không thể tải b1.png: " << IMG_GetError() << std::endl;
        isRunning = false;
        return;
    }
    buttonStartTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!buttonStartTexture) {
        std::cerr << "Không thể tạo texture cho b1.png: " << IMG_GetError() << std::endl;
        isRunning = false;
        return;
    }

    // Tải nút play (b2.png)
    surface = IMG_Load("b2.png");
    if (!surface) {
        std::cerr << "Không thể tải b2.png: " << IMG_GetError() << std::endl;
        isRunning = false;
        return;
    }
    buttonPlayTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!buttonPlayTexture) {
        std::cerr << "Không thể tạo texture cho b2.png: " << IMG_GetError() << std::endl;
        isRunning = false;
        return;
    }
}

void Game::run() {
    while (isRunning) {
        processEvents();
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        update(mouseX, mouseY);
        render();
        SDL_Delay(1000 / 60); // 60 FPS
    }
}

void Game::processEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        } else if (event.type == SDL_MOUSEBUTTONDOWN && currentState == MENU) {
            if (event.button.button == SDL_BUTTON_LEFT && isButtonPlay) {
                currentState = PLAYING;
            }
        } else if (currentState == PLAYING && !isPaused) {
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:  // Mũi tên trái
                    case SDLK_a:     // Phím A
                        characterDx = -1; // Di chuyển sang trái
                        break;
                    case SDLK_RIGHT: // Mũi tên phải
                    case SDLK_d:     // Phím D
                        characterDx = 1;  // Di chuyển sang phải
                        break;
                    case SDLK_SPACE: // Phím Space
                        isCasting = true; // Thả dây
                        character->castLine(isCasting);
                        break;
                    case SDLK_r: // Phím R để tạm dừng
                        isPaused = true;
                        break;
                }
            } else if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                    case SDLK_a:
                    case SDLK_RIGHT:
                    case SDLK_d:
                        characterDx = 0; // Dừng di chuyển
                        break;
                    case SDLK_SPACE: // Thả Space
                        isCasting = false; // Thu dây
                        character->castLine(isCasting);
                        break;
                }
            }
        } else if (currentState == PLAYING && isPaused) {
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r) {
                isPaused = false; // Tiếp tục
            }
        }
    }
}

void Game::update(int mouseX, int mouseY) {
    if (currentState == MENU) {
        // Kiểm tra chuột có trong vùng nút không
        if (mouseX >= buttonRect.x && mouseX <= buttonRect.x + buttonRect.w &&
            mouseY >= buttonRect.y && mouseY <= buttonRect.y + buttonRect.h) {
            isButtonPlay = true; // Hiển thị b2.png
        } else {
            isButtonPlay = false; // Hiển thị b1.png
        }
    } else if (currentState == PLAYING && !isPaused) {
        character->update(characterDx); // Cập nhật vị trí nhân vật và cần câu
    }
}

void Game::render() {
    SDL_RenderClear(renderer);

    if (currentState == MENU) {
        SDL_RenderCopy(renderer, bgTexture, nullptr, nullptr); // back1.png
        SDL_RenderCopy(renderer, isButtonPlay ? buttonPlayTexture : buttonStartTexture, nullptr, &buttonRect);
    } else if (currentState == PLAYING) {
        SDL_RenderCopy(renderer, bgGameTexture, nullptr, nullptr); // back2.png
        character->render(renderer); // Vẽ nhân vật, cần câu và dây
    }

    SDL_RenderPresent(renderer);
}

void Game::cleanup() {
    if (character) {
        delete character;
    }
    if (buttonPlayTexture) SDL_DestroyTexture(buttonPlayTexture);
    if (buttonStartTexture) SDL_DestroyTexture(buttonStartTexture);
    if (bgGameTexture) SDL_DestroyTexture(bgGameTexture);
    if (bgTexture) SDL_DestroyTexture(bgTexture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}