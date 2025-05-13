#include "Game.h"
#include <iostream>
#include <random>
#include <cmath>
#include <sstream>

Game::Game()
    : window(nullptr), renderer(nullptr), bgTexture(nullptr), bgGameTexture(nullptr),
      buttonStartTexture(nullptr), buttonPlayTexture(nullptr), isButtonPlay(false),
      isRunning(false), isPaused(false), currentState(MENU), character(nullptr),
      characterDx(0), isCasting(false), score(0), timeLeft(75.0f), aim(0),
      hasWon(false), font(nullptr), showRestartText(true), blinkTimer(0.0f),
      explosionTexture(nullptr), isExploding(false), explosionFrame(0), explosionTimer(0.0f),
      explosionPosition({0, 0}), isRestarting(false), restartTimer(0.0f),
      winSound(nullptr), loseSound(nullptr), bombSound(nullptr), goldSound(nullptr),
      spaceClickSound(nullptr), movementSoundChannel(-1) {
    buttonRect = { (1200 - 200) / 2, (600) / 2, 200, 100 };
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> aimDist(1550, 1660);
    aim = aimDist(gen);
}

Game::~Game() {
    cleanup();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "Không thể khởi tạo SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() < 0) {
        std::cerr << "Không thể khởi tạo SDL_ttf: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Không thể khởi tạo SDL_mixer: " << Mix_GetError() << std::endl;
        TTF_Quit();
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
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Không thể tạo renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "Không thể khởi tạo SDL_image: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
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
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    character = new Character(renderer);

    SDL_Surface* surface = IMG_Load("Explode.png");
    if (!surface) {
        std::cerr << "Không thể tải Explode.png: " << IMG_GetError() << std::endl;
        cleanup();
        return false;
    }
    explosionTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!explosionTexture) {
        std::cerr << "Không thể tạo texture cho Explode.png: " << IMG_GetError() << std::endl;
        cleanup();
        return false;
    }

    loadResources();
    loadSounds();
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

void Game::loadSounds() {
    winSound = Mix_LoadWAV("Winning sound.mp3");
    if (!winSound) {
        std::cerr << "Không thể tải Winning sound.mp3: " << Mix_GetError() << std::endl;
    }
    loseSound = Mix_LoadWAV("Losing sound.mp3");
    if (!loseSound) {
        std::cerr << "Không thể tải Losing sound.mp3: " << Mix_GetError() << std::endl;
    }
    bombSound = Mix_LoadWAV("bomb explode.mp3");
    if (!bombSound) {
        std::cerr << "Không thể tải bomb explode.mp3: " << Mix_GetError() << std::endl;
    }
    goldSound = Mix_LoadWAV("GOLD OR MYSBOX.mp3");
    if (!goldSound) {
        std::cerr << "Không thể tải GOLD OR MYSBOX.mp3: " << Mix_GetError() << std::endl;
    }
    spaceClickSound = Mix_LoadWAV("SPACE OR CLICK.mp3");
    if (!spaceClickSound) {
        std::cerr << "Không thể tải SPACE OR CLICK.mp3: " << Mix_GetError() << std::endl;
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
        { BOMB, bombCountDist(gen), 80 }
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
                if (spaceClickSound) {
                    Mix_PlayChannel(-1, spaceClickSound, 0);
                }
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
                        if (spaceClickSound) {
                            Mix_PlayChannel(-1, spaceClickSound, 0);
                        }
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
        } else if (currentState == GAME_OVER && !isRestarting) {
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                isRestarting = true;
                restartTimer = 5.0f;
                if (spaceClickSound) {
                    Mix_PlayChannel(-1, spaceClickSound, 0);
                }
            }
        }
    }
}

void Game::update(int mouseX, int mouseY) {
    static bool playedWinLoseSound = false;

    if (currentState == MENU) {
        if (mouseX >= buttonRect.x && mouseX <= buttonRect.x + buttonRect.w &&
            mouseY >= buttonRect.y && mouseY <= buttonRect.y + buttonRect.h) {
            isButtonPlay = true;
        } else {
            isButtonPlay = false;
        }
    } else if (currentState == PLAYING && !isPaused) {
        character->update(characterDx);

        // Handle movement sound
        if (characterDx != 0 && spaceClickSound && movementSoundChannel == -1) {
            // Start playing the movement sound in a loop
            movementSoundChannel = Mix_PlayChannel(-1, spaceClickSound, -1); // -1 for infinite loops
            if (movementSoundChannel == -1) {
                std::cerr << "Không thể phát SPACE OR CLICK.mp3 cho di chuyển: " << Mix_GetError() << std::endl;
            }
        } else if (characterDx == 0 && movementSoundChannel != -1) {
            // Stop the movement sound
            Mix_HaltChannel(movementSoundChannel);
            movementSoundChannel = -1;
        }

        timeLeft -= 1.0f / 60.0f;
        if (timeLeft <= 0) {
            timeLeft = 0;
            hasWon = score >= aim;
            currentState = GAME_OVER;
            playedWinLoseSound = false;
            if (movementSoundChannel != -1) {
                Mix_HaltChannel(movementSoundChannel);
                movementSoundChannel = -1;
            }
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
                    playedWinLoseSound = false;
                    if (movementSoundChannel != -1) {
                        Mix_HaltChannel(movementSoundChannel);
                        movementSoundChannel = -1;
                    }
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
                    explosionPosition = { itemCenterX, itemCenterY };
                    isExploding = true;
                    explosionFrame = 0;
                    explosionTimer = 0.0f;
                    character->resetCatch();
                    if (bombSound) {
                        Mix_PlayChannel(-1, bombSound, 0);
                    }
                    it = items.erase(it);
                    break;
                } else if (type == MYSTERY_BOX && distance < 17) {
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_real_distribution<> bombChance(0.0, 1.0);
                    if (bombChance(gen) < 0.3) {
                        explosionPosition = { itemCenterX, itemCenterY };
                        isExploding = true;
                        explosionFrame = 0;
                        explosionTimer = 0.0f;
                        character->resetCatch();
                        if (bombSound) {
                            Mix_PlayChannel(-1, bombSound, 0);
                        }
                        it = items.erase(it);
                        break;
                    } else {
                        character->catchItem();
                        character->setCaughtItem(*it);
                        if (goldSound) {
                            Mix_PlayChannel(-1, goldSound, 0);
                        }
                        it = items.erase(it);
                        break;
                    }
                } else if ((type == GOLD || type == BIG_GOLD) && distance < 17) {
                    character->catchItem();
                    character->setCaughtItem(*it);
                    if (goldSound) {
                        Mix_PlayChannel(-1, goldSound, 0);
                    }
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
                    playedWinLoseSound = false;
                    if (movementSoundChannel != -1) {
                        Mix_HaltChannel(movementSoundChannel);
                        movementSoundChannel = -1;
                    }
                }
            }
        }
    } else if (currentState == GAME_OVER) {
        if (!playedWinLoseSound) {
            if (hasWon && winSound) {
                Mix_PlayChannel(-1, winSound, 0);
            } else if (!hasWon && loseSound) {
                Mix_PlayChannel(-1, loseSound, 0);
            }
            playedWinLoseSound = true;
        }

        if (isRestarting) {
            restartTimer -= 1.0f / 60.0f;
            if (restartTimer <= 0.0f) {
                score = 0;
                timeLeft = 75.0f;
                hasWon = false;
                currentState = PLAYING;
                isRestarting = false;
                isCasting = false;
                character->reset();
                generateItems();
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> aimDist(1550, 1660);
                aim = aimDist(gen);
                playedWinLoseSound = false;
                if (movementSoundChannel != -1) {
                    Mix_HaltChannel(movementSoundChannel);
                    movementSoundChannel = -1;
                }
            }
        }
        blinkTimer += 1.0f / 60.0f;
        if (blinkTimer >= 0.5f) {
            showRestartText = !showRestartText;
            blinkTimer = 0.0f;
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

        if (isExploding) {
            SDL_Rect srcRect = { explosionFrame * 72, 0, 72, 88 };
            SDL_Rect dstRect = { explosionPosition.x - 40, explosionPosition.y - 44, 80, 88 };
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
        if (showRestartText && !isRestarting) {
            SDL_Color white = { 255, 255, 255, 255 };
            renderText("Please press SPACE to restart", 0, 300, white, true);
        }
        if (isRestarting) {
            SDL_Color white = { 255, 255, 255, 255 };
            std::stringstream restartText;
            restartText << "Restarting in " << static_cast<int>(std::ceil(restartTimer)) << "s";
            renderText(restartText.str(), 0, 350, white, true);
        }
    }

    SDL_RenderPresent(renderer);
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
    if (winSound) Mix_FreeChunk(winSound);
    if (loseSound) Mix_FreeChunk(loseSound);
    if (bombSound) Mix_FreeChunk(bombSound);
    if (goldSound) Mix_FreeChunk(goldSound);
    if (spaceClickSound) Mix_FreeChunk(spaceClickSound);
    if (movementSoundChannel != -1) {
        Mix_HaltChannel(movementSoundChannel);
    }
    if (font) TTF_CloseFont(font);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    Mix_CloseAudio();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}