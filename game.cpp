#include "Game.h"
#include <iostream>
#include <sstream>

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
    , items(nullptr)
    , characterDx(0)
    , isCasting(false)
    , score(0)
    , font(nullptr)
    , scoreTexture(nullptr) {
    buttonRect = { (1200 - 200) / 2, (600 - 100) / 2, 200, 100 };
    scoreRect = { 0, 0, 0, 0 };
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

    window = SDL_CreateWindow("Game Fishing",
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

    font = TTF_OpenFont("Arial.ttf", 24);
    if (!font) {
        std::cerr << "Không thể tải font Arial.ttf: " << TTF_GetError() << std::endl;
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
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    items = new Items(1,renderer);

    loadResources();
    updateScoreTexture();
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

void Game::updateScoreTexture() {
    if (scoreTexture) {
        SDL_DestroyTexture(scoreTexture);
        scoreTexture = nullptr;
    }

    std::stringstream ss;
    ss << "Score: " << score;
    std::string scoreText = ss.str();

    SDL_Color textColor = { 255, 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    if (!textSurface) {
        std::cerr << "Không thể tạo surface cho điểm số: " << TTF_GetError() << std::endl;
        return;
    }

    scoreTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!scoreTexture) {
        std::cerr << "Không thể tạo texture cho điểm số: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(textSurface);
        return;
    }

    scoreRect.w = textSurface->w;
    scoreRect.h = textSurface->h;
    scoreRect.x = 1200 - scoreRect.w - 20;
    scoreRect.y = 20;

    SDL_FreeSurface(textSurface);
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
        character->checkCollisionWithItems(items->getItems(), score, items);
        items->update();
        updateScoreTexture();
    }
}

void Game::render() {
    SDL_RenderClear(renderer);

    if (currentState == MENU) {
        SDL_RenderCopy(renderer, bgTexture, nullptr, nullptr);
        SDL_RenderCopy(renderer, isButtonPlay ? buttonPlayTexture : buttonStartTexture, nullptr, &buttonRect);
    } else if (currentState == PLAYING) {
        SDL_RenderCopy(renderer, bgGameTexture, nullptr, nullptr);
        items->render(renderer);
        character->render(renderer);
        if (scoreTexture) {
            SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);
        }
    }

    SDL_RenderPresent(renderer);
}

void Game::cleanup() {
    if (scoreTexture) SDL_DestroyTexture(scoreTexture);
    if (font) TTF_CloseFont(font);
    if (items) delete items;
    if (character) delete character;
    if (buttonPlayTexture) SDL_DestroyTexture(buttonPlayTexture);
    if (buttonStartTexture) SDL_DestroyTexture(buttonStartTexture);
    if (bgGameTexture) SDL_DestroyTexture(bgGameTexture);
    if (bgTexture) SDL_DestroyTexture(bgTexture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}