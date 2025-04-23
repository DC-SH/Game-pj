#include "game.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

Game::Game() : window(nullptr), renderer(nullptr), bigGoldTexture(nullptr), miniGoldTexture(nullptr), bombTexture(nullptr), mysteryBoxTexture(nullptr), explosionTexture(nullptr), backgroundTexture(nullptr), font(nullptr), score(0), boundaryY(200), currentRound(0), gameOver(false), timeLeft(0), maxTime(0), scoreTexture(nullptr), timeTexture(nullptr), lastScore(-1), lastTimeLeft(-1) {
    roundTimes[0] = 60;
    roundTimes[1] = 45;
    roundTimes[2] = 30;
    roundScores[0] = 1500;
    roundScores[1] = 3000;
    roundScores[2] = 4700;
    scoreRect = { 10, 10, 0, 0 };
    timeRect = { 0, 10, 0, 0 };
}

Game::~Game() {
    SDL_DestroyTexture(bigGoldTexture);
    SDL_DestroyTexture(miniGoldTexture);
    SDL_DestroyTexture(bombTexture);
    SDL_DestroyTexture(mysteryBoxTexture);
    SDL_DestroyTexture(explosionTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(scoreTexture);
    SDL_DestroyTexture(timeTexture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL_Init failed: " << SDL_GetError() << endl;
        return false;
    }
    if (TTF_Init() == -1) {
        cerr << "TTF_Init failed: " << TTF_GetError() << endl;
        return false;
    }

    window = SDL_CreateWindow("Gold Miner", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        cerr << "SDL_CreateWindow failed: " << SDL_GetError() << endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << endl;
        return false;
    }

    bigGoldTexture = IMG_LoadTexture(renderer, "big_gold.png"); // Sửa tên
    if (!bigGoldTexture) {
        cerr << "Failed to load big_gold.png: " << IMG_GetError() << endl;
        return false;
    }

    miniGoldTexture = IMG_LoadTexture(renderer, "mini_gold.png"); // Sửa tên
    if (!miniGoldTexture) {
        cerr << "Failed to load mini_gold.png: " << IMG_GetError() << endl;
        return false;
    }

    bombTexture = IMG_LoadTexture(renderer, "bomb.png");
    if (!bombTexture) {
        cerr << "Failed to load bomb.png: " << IMG_GetError() << endl;
        return false;
    }

    mysteryBoxTexture = IMG_LoadTexture(renderer, "mystery_box.png"); // Sửa tên
    if (!mysteryBoxTexture) {
        cerr << "Failed to load mystery_box.png: " << IMG_GetError() << endl;
        return false;
    }

    explosionTexture = IMG_LoadTexture(renderer, "explosion.png");
    if (!explosionTexture) {
        cerr << "Failed to load explosion.png: " << IMG_GetError() << endl;
        return false;
    }

    backgroundTexture = IMG_LoadTexture(renderer, "background 2.png");
    if (!backgroundTexture) {
        cerr << "Failed to load background 2.png: " << IMG_GetError() << endl;
        return false;
    }

    font = TTF_OpenFont("arial.ttf", 24);
    if (!font) {
        cerr << "Failed to load font arial.ttf: " << TTF_GetError() << endl;
        return false;
    }

    spawnItems(1);
    startTime = SDL_GetTicks();
    currentRound = 1;
    timeLeft = roundTimes[0];
    maxTime = roundTimes[0];

    updateScoreTexture();
    updateTimeTexture();

    return true;
}

void Game::spawnItems(int level) {
    golds.clear();
    srand(static_cast<unsigned int>(time(nullptr)));

    if (level == 1) {
        for (int i = 0; i < 3; ++i) {
            Gold gold;
            gold.rect = { rand() % 600 + 100, rand() % 200 + 300, 50, 50 };
            gold.isBomb = false;
            gold.isPickedUp = false;
            gold.isMysteryBox = false;
            gold.value = 100;
            golds.push_back(gold);
        }
        Gold bomb;
        bomb.rect = { rand() % 600 + 100, rand() % 200 + 300, 50, 50 };
        bomb.isBomb = true;
        bomb.isPickedUp = false;
        bomb.isMysteryBox = false;
        bomb.value = 0;
        golds.push_back(bomb);
    } else if (level == 2) {
        for (int i = 0; i < 2; ++i) {
            Gold gold;
            gold.rect = { rand() % 600 + 100, rand() % 200 + 300, 50, 50 };
            gold.isBomb = false;
            gold.isPickedUp = false;
            gold.isMysteryBox = false;
            gold.value = 100;
            golds.push_back(gold);
        }
        for (int i = 0; i < 2; ++i) {
            Gold bigGold;
            bigGold.rect = { rand() % 600 + 100, rand() % 200 + 300, 50, 50 };
            bigGold.isBomb = false;
            bigGold.isPickedUp = false;
            bigGold.isMysteryBox = false;
            bigGold.value = 175;
            golds.push_back(bigGold);
        }
        Gold bomb;
        bomb.rect = { rand() % 600 + 100, rand() % 200 + 300, 50, 50 };
        bomb.isBomb = true;
        bomb.isPickedUp = false;
        bomb.isMysteryBox = false;
        bomb.value = 0;
        golds.push_back(bomb);
        Gold mysteryBox;
        mysteryBox.rect = { rand() % 600 + 100, rand() % 200 + 300, 50, 50 };
        mysteryBox.isBomb = false;
        mysteryBox.isPickedUp = false;
        mysteryBox.isMysteryBox = true;
        mysteryBox.value = 0;
        golds.push_back(mysteryBox);
    } else if (level == 3) {
        for (int i = 0; i < 2; ++i) {
            Gold gold;
            gold.rect = { rand() % 600 + 100, rand() % 200 + 300, 50, 50 };
            gold.isBomb = false;
            gold.isPickedUp = false;
            gold.isMysteryBox = false;
            gold.value = 100;
            golds.push_back(gold);
        }
        for (int i = 0; i < 3; ++i) {
            Gold bigGold;
            bigGold.rect = { rand() % 600 + 100, rand() % 200 + 300, 50, 50 };
            bigGold.isBomb = false;
            bigGold.isPickedUp = false;
            bigGold.isMysteryBox = false;
            bigGold.value = 175;
            golds.push_back(bigGold);
        }
        for (int i = 0; i < 2; ++i) {
            Gold bomb;
            bomb.rect = { rand() % 600 + 100, rand() % 200 + 300, 50, 50 };
            bomb.isBomb = true;
            bomb.isPickedUp = false;
            bomb.isMysteryBox = false;
            bomb.value = 0;
            golds.push_back(bomb);
        }
        for (int i = 0; i < 2; ++i) {
            Gold mysteryBox;
            mysteryBox.rect = { rand() % 600 + 100, rand() % 200 + 300, 50, 50 };
            mysteryBox.isBomb = false;
            mysteryBox.isPickedUp = false;
            mysteryBox.isMysteryBox = true;
            mysteryBox.value = 0;
            golds.push_back(mysteryBox);
        }
    }
}

void Game::update() {
    if (gameOver) return;

    elapsedTime = (SDL_GetTicks() - startTime) / 1000;
    timeLeft = roundTimes[currentRound - 1] - elapsedTime;

    if (score != lastScore) {
        updateScoreTexture();
    }
    if (timeLeft != lastTimeLeft) {
        updateTimeTexture();
    }

    if (timeLeft <= 0 || allItemsPickedUp()) {
        if (score >= roundScores[currentRound - 1] && currentRound < 3) {
            currentRound++;
            spawnItems(currentRound);
            startTime = SDL_GetTicks();
            timeLeft = roundTimes[currentRound - 1];
            maxTime = roundTimes[currentRound - 1];
        } else {
            gameOver = true;
        }
    }
}

void Game::updateScoreTexture() {
    if (scoreTexture) {
        SDL_DestroyTexture(scoreTexture);
    }
    string scoreText = "Score: " + to_string(score);
    SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), { 255, 255, 255 });
    if (!scoreSurface) {
        cerr << "TTF_RenderText_Solid failed for score: " << TTF_GetError() << endl;
        return;
    }
    scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    if (!scoreTexture) {
        cerr << "SDL_CreateTextureFromSurface failed for score: " << SDL_GetError() << endl;
    }
    scoreRect = { 10, 10, scoreSurface->w, scoreSurface->h };
    SDL_FreeSurface(scoreSurface);
    lastScore = score;
}

void Game::updateTimeTexture() {
    if (timeTexture) {
        SDL_DestroyTexture(timeTexture);
    }
    string timeText = "Time: " + to_string(timeLeft);
    SDL_Surface* timeSurface = TTF_RenderText_Solid(font, timeText.c_str(), { 255, 255, 255 });
    if (!timeSurface) {
        cerr << "TTF_RenderText_Solid failed for time: " << TTF_GetError() << endl;
        return;
    }
    timeTexture = SDL_CreateTextureFromSurface(renderer, timeSurface);
    if (!timeTexture) {
        cerr << "SDL_CreateTextureFromSurface failed for time: " << SDL_GetError() << endl;
    }
    timeRect = { 700 - timeSurface->w, 10, timeSurface->w, timeSurface->h };
    SDL_FreeSurface(timeSurface);
    lastTimeLeft = timeLeft;
}

void Game::render() {
    SDL_RenderClear(renderer);
    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    }

    for (const Gold& gold : golds) {
        if (!gold.isPickedUp) {
            if (gold.isBomb && bombTexture) {
                SDL_RenderCopy(renderer, bombTexture, NULL, &gold.rect);
            } else if (gold.isMysteryBox && mysteryBoxTexture) {
                SDL_RenderCopy(renderer, mysteryBoxTexture, NULL, &gold.rect);
            } else if (gold.value == 175 && bigGoldTexture) {
                SDL_RenderCopy(renderer, bigGoldTexture, NULL, &gold.rect);
            } else if (miniGoldTexture) {
                SDL_RenderCopy(renderer, miniGoldTexture, NULL, &gold.rect);
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect boundary = { 0, boundaryY, 800, 2 };
    SDL_RenderFillRect(renderer, &boundary);

    if (scoreTexture) {
        SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
    }
    if (timeTexture) {
        SDL_RenderCopy(renderer, timeTexture, NULL, &timeRect);
    }

    SDL_RenderPresent(renderer);
}