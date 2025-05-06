#include "Game.h"
#include <iostream>
#include <random>
#include <cmath>
#include <sstream>

Game::Game()
    : window(nullptr), renderer(nullptr), bgTexture(nullptr), bgGameTexture(nullptr),
      buttonStartTexture(nullptr), buttonPlayTexture(nullptr), isButtonPlay(false),
      isRunning(false), isPaused(false), currentState(MENU), character(nullptr),
      characterDx(0), isCasting(false), score(0), timeLeft(75.0f), hasWon(false),
      font(nullptr), showRestartText(true), blinkTimer(0.0f), explosionTexture(nullptr),
      isExploding(false), explosionFrame(0), explosionTimer(0.0f), explosionPosition({0, 0}) {
    buttonRect = { (1200 - 200 ) / 2, (600) / 2, 200, 100 };
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> aimDist(1550, 1660);
    aim = aimDist(gen);
}

Game::~Game() {
    cleanup();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Không thể khởi tạo SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() < 0) {
        std::cerr << "Không thể khởi tạo SDL_ttf: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    window = SDL_CreateWindow("Gold mining",
                             SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED,
                             1200, 600,
                             SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Không thể tạo cửa sổ: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Không thể tạo renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "Không thể khởi tạo SDL_image: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    font = TTF_OpenFont("font.ttf", 24);
    if (!font) {
        std::cerr << "Không thể tải font.ttf: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    character = new Character(renderer);
    if (!character->loadTexture("chill.png", renderer)) {
        std::cerr << "Lỗi: Không tìm thấy file chill.png hoặc file không đúng định dạng!" << std::endl;
        delete character;
        character = nullptr;
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    SDL_Surface* surface = IMG_Load("explosionsprite.png");
    if (!surface) {
        std::cerr << "Không thể tải explosionsprite.png: " << IMG_GetError() << std::endl;
        cleanup();
        return false;
    }
    explosionTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!explosionTexture) {
        std::cerr << "Không thể tạo texture cho explosionsprite.png: " << IMG_GetError() << std::endl;
        cleanup();
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
    std::uniform_int_distribution<> bombCountDist(4, 6);

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
        { MYSTERY_BOX, 2, 40 },
        { BOMB, bombCountDist(gen), 80 } // Kích thước bom tăng lên 80x80
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
        } else if (currentState == GAME_OVER) {
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                score = 0;
                timeLeft = 75.0f;
                hasWon = false;
                currentState = PLAYING;
                generateItems();
                character->resetCatch();
                character->setCaughtItem(nullptr);
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> aimDist(1550, 1660);
                aim = aimDist(gen);
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

        timeLeft -= 1.0f / 60.0f;
        if (timeLeft <= 0) {
            timeLeft = 0;
            hasWon = score >= aim;
            currentState = GAME_OVER;
        }

        if (isExploding) {
            explosionTimer += 1.0f / 60.0f;
            if (explosionTimer >= EXPLOSION_FRAME_TIME) {
                explosionFrame++;
                explosionTimer = 0.0f;
                if (explosionFrame >= EXPLOSION_FRAMES) {
                    isExploding = false;
                    hasWon = false;
                    currentState = GAME_OVER;
                }
            }
            return;
        }

        if (!character->hasCaughtItem() && !character->isItemCaught()) {
            SDL_Point hookPos = character->getHookPosition();
            for (auto it = items.begin(); it != items.end();) {
                SDL_Rect itemRect = (*it)->getRect();
                ItemType type = (*it)->getType();
                int itemCenterX = itemRect.x + itemRect.w / 2;
                int itemCenterY = itemRect.y + itemRect.h / 2;
                double distance = sqrt(pow(hookPos.x - itemCenterX, 2) + pow(hookPos.y - itemCenterY, 2));
                if (type == BOMB && distance < 17) {
                    // Lưu vị trí bom để hiển thị hiệu ứng nổ
                    explosionPosition = { itemCenterX, itemCenterY };
                    isExploding = true;
                    explosionFrame = 0;
                    explosionTimer = 0.0f;
                    character->resetCatch();
                    it = items.erase(it);
                    break;
                } else if ((type == GOLD || type == BIG_GOLD || type == MYSTERY_BOX) && distance < 17) {
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
            if (itemRect.y <= 310) {
                ItemType type = caughtItem->getType();
                if (type == BIG_GOLD) {
                    score += 150;
                } else if (type == GOLD) {
                    score += 100;
                } else if (type == MYSTERY_BOX) {
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_int_distribution<> dist(50, 200);
                    score += dist(gen);
                }
                delete caughtItem;
                character->setCaughtItem(nullptr);
                if (score >= aim) {
                    hasWon = true;
                    currentState = GAME_OVER;
                }
            }
        }
    } else if (currentState == GAME_OVER) {
        blinkTimer += 1.0f / 60.0f;
        if (blinkTimer >= 0.5f) {
            showRestartText = !showRestartText;
            blinkTimer = 0.0f;
        }
    }
}

void Game::renderText(const std::string& text, int x, int y, SDL_Color color, bool centered) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!surface) {
        std::cerr << "Không thể render văn bản: " << TTF_GetError() << std::endl;
        return;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        std::cerr << "Không thể tạo texture văn bản: " << TTF_GetError() << std::endl;
        return;
    }

    int width, height;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    SDL_Rect dstRect = { x, y, width, height };
    if (centered) {
        dstRect.x = (1200 - width) / 2;
    }
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
    SDL_DestroyTexture(texture);
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

        if (isExploding) {
            SDL_Rect srcRect = { explosionFrame * (512 / EXPLOSION_FRAMES), 0, 512 / EXPLOSION_FRAMES, 512 / EXPLOSION_FRAMES };
            SDL_Rect dstRect = { explosionPosition.x - 50, explosionPosition.y - 50, 100, 100 }; // Hiệu ứng nổ tại vị trí bom
            SDL_RenderCopy(renderer, explosionTexture, &srcRect, &dstRect);
        }

        SDL_Color white = { 255, 255, 255, 255 };
        std::stringstream aimText;
        aimText << "aim: " << aim;
        renderText(aimText.str(), 10, 10, white);
        std::stringstream scoreText;
        scoreText << "score: " << score;
        renderText(scoreText.str(), 10, 40, white);

        std::stringstream timeText;
        timeText << "time: " << static_cast<int>(timeLeft) << "s";
        int textWidth = 100;
        renderText(timeText.str(), 1200 - textWidth - 10, 10, white);
    } else if (currentState == GAME_OVER) {
        SDL_RenderCopy(renderer, bgGameTexture, nullptr, nullptr);
        SDL_Color color = hasWon ? SDL_Color{ 255, 0, 0, 255 } : SDL_Color{ 139, 69, 19, 255 };
        std::string resultText = hasWon ? "YOU WIN" : "YOU LOSE";
        renderText(resultText, 0, 250, color, true);
        if (showRestartText) {
            SDL_Color white = { 255, 255, 255, 255 };
            renderText("Please press SPACE to restart", 0, 300, white, true);
        }
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
    if (explosionTexture) SDL_DestroyTexture(explosionTexture);
    if (font) TTF_CloseFont(font);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}