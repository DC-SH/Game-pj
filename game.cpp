#include "Game.h"
#include <iostream>
#include <random>
#include <cmath>

Game::Game()
    : window(nullptr), renderer(nullptr), bgTexture(nullptr), bgGameTexture(nullptr),
      buttonStartTexture(nullptr), buttonPlayTexture(nullptr), isButtonPlay(false),
      isRunning(false), isPaused(false), currentState(MENU), character(nullptr),
      characterDx(0), isCasting(false) {
    buttonRect = { (1200 - 200) / 2, (600 - 100) / 2, 200, 100 };
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
    generateItems();
    isRunning = true;
    return true;
}

void Game::loadResources() {
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

void Game::generateItems() {
    for (auto item : items) {
        delete item;
    }
    items.clear();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> xDist(75, 1125);
    std::uniform_int_distribution<> yDist(400, 550);

    const int minDistance = 70;
    std::vector<SDL_Point> positions;

    struct ItemToGenerate {
        ItemType type;
        int count;
        int width;
    };
    std::vector<ItemToGenerate> itemsToGenerate = {
        { BIG_GOLD, 6, 100 },
        { GOLD, 5, 150 },
        { MYSTERY_BOX, 2, 40 }
    };

    for (const auto& itemGen : itemsToGenerate) {
        for (int i = 0; i < itemGen.count; ++i) {
            bool validPosition = false;
            int x, y;
            int attempts = 0;
            const int maxAttempts = 100;

            while (!validPosition && attempts < maxAttempts) {
                x = xDist(gen);
                y = yDist(gen);
                validPosition = true;

                for (const auto& pos : positions) {
                    double distance = std::sqrt(std::pow(x - pos.x, 2) + std::pow(y - pos.y, 2));
                    if (distance < minDistance) {
                        validPosition = false;
                        break;
                    }
                }

                attempts++;
            }

            if (validPosition) {
                int offset = itemGen.width / 2;
                Item* item = new Item(renderer, itemGen.type, x - offset, y - offset);
                items.push_back(item);
                positions.push_back({ x, y });
            } else {
                std::cerr << "Cảnh báo: Không tìm được vị trí hợp lệ cho vật phẩm sau " << maxAttempts << " lần thử" << std::endl;
            }
        }
    }
}

void Game::run() {
    while (isRunning) {
        processEvents();
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        update(mouseX, mouseY);
        render();
        SDL_Delay(1000 / 60);
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
                    case SDLK_LEFT:
                    case SDLK_a:
                        characterDx = -1;
                        break;
                    case SDLK_RIGHT:
                    case SDLK_d:
                        characterDx = 1;
                        break;
                    case SDLK_SPACE:
                        isCasting = true;
                        character->castLine(isCasting);
                        break;
                    case SDLK_r:
                        isPaused = true;
                        break;
                }
            } else if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                    case SDLK_a:
                    case SDLK_RIGHT:
                    case SDLK_d:
                        characterDx = 0;
                        break;
                    case SDLK_SPACE:
                        isCasting = false;
                        character->castLine(isCasting);
                        break;
                }
            }
        } else if (currentState == PLAYING && isPaused) {
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r) {
                isPaused = false;
            }
        }
    }
}

void Game::update(int mouseX, int mouseY) {
    if (currentState == MENU) {
        if (mouseX >= buttonRect.x && mouseX <= buttonRect.x + buttonRect.w &&
            mouseY >= buttonRect.y && mouseY <= buttonRect.y + buttonRect.h) {
            isButtonPlay = true;
        } else {
            isButtonPlay = false;
        }
    } else if (currentState == PLAYING && !isPaused) {
        character->update(characterDx);

        if (!character->hasCaughtItem() && !character->isItemCaught()) {
            SDL_Point hookPos = character->getHookPosition();
            for (auto it = items.begin(); it != items.end();) {
                SDL_Rect itemRect = (*it)->getRect();
                ItemType type = (*it)->getType();
                int itemCenterX = itemRect.x + itemRect.w / 2;
                int itemCenterY = itemRect.y + itemRect.h / 2;
                double distance = sqrt(pow(hookPos.x - itemCenterX, 2) + pow(hookPos.y - itemCenterY, 2));
                if ((type == GOLD || type == BIG_GOLD || type == MYSTERY_BOX) && distance < 17) { // Thay từ 5 thành 17
                    character->catchItem();
                    character->setCaughtItem(*it);
                    it = items.erase(it);
                    break;
                } else {
                    ++it;
                }
            }
        }

        Item* caughtItem = character->getCaughtItem();
        if (caughtItem) {
            SDL_Rect itemRect = caughtItem->getRect();
            if (itemRect.y <= 310) { // Đã thay đổi MOVE_AREA_BOTTOM thành 310 trước đó
                delete caughtItem;
                character->setCaughtItem(nullptr);
            }
        }
    }
}

void Game::render() {
    SDL_RenderClear(renderer);

    if (currentState == MENU) {
        SDL_RenderCopy(renderer, bgTexture, nullptr, nullptr);
        SDL_RenderCopy(renderer, isButtonPlay ? buttonPlayTexture : buttonStartTexture, nullptr, &buttonRect);
    } else if (currentState == PLAYING) {
        SDL_RenderCopy(renderer, bgGameTexture, nullptr, nullptr);
        for (const auto& item : items) {
            item->render(renderer);
        }
        Item* caughtItem = character->getCaughtItem();
        if (caughtItem) {
            caughtItem->render(renderer);
        }
        character->render(renderer);
    }

    SDL_RenderPresent(renderer);
}

void Game::cleanup() {
    for (auto item : items) {
        delete item;
    }
    items.clear();
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