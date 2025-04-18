#include "game.h"
#include <iostream>
#include <cstdlib>

using namespace std;

Game::Game() : window(NULL), renderer(NULL), bigGoldTexture(NULL), miniGoldTexture(NULL), bombTexture(NULL), mysteryBoxTexture(NULL), explosionTexture(NULL), backgroundTexture(NULL), font(NULL), score(0), boundaryY(100), startTime(0), elapsedTime(0), currentRound(0), gameOver(false), timeLeft(0) {
    roundTimes[0] = 85; roundTimes[1] = 120; roundTimes[2] = 160;
    roundScores[0] = 1500; roundScores[1] = 3200; roundScores[2] = 4700;
}

Game::~Game() {
    SDL_DestroyTexture(bigGoldTexture);
    SDL_DestroyTexture(miniGoldTexture);
    SDL_DestroyTexture(bombTexture);
    SDL_DestroyTexture(mysteryBoxTexture);
    SDL_DestroyTexture(explosionTexture);
    SDL_DestroyTexture(backgroundTexture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    window = SDL_CreateWindow("Gold Mining Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;

    if (IMG_Init(IMG_INIT_PNG) == 0) return false;
    if (TTF_Init() == -1) return false;

    bigGoldTexture = IMG_LoadTexture(renderer, "big_gold.png");
    miniGoldTexture = IMG_LoadTexture(renderer, "mini_gold.png");
    bombTexture = IMG_LoadTexture(renderer, "bomb.png");
    mysteryBoxTexture = IMG_LoadTexture(renderer, "mystery_box.png");
    explosionTexture = IMG_LoadTexture(renderer, "explosion.png");
    backgroundTexture = IMG_LoadTexture(renderer, "background 2.png");
    font = TTF_OpenFont("arial.ttf", 24);

    if (!bigGoldTexture || !miniGoldTexture || !bombTexture || !mysteryBoxTexture || !explosionTexture || !backgroundTexture || !font) {
        cerr << "Failed to load resources!" << endl;
        return false;
    }

    spawnItems(1);
    startTime = SDL_GetTicks();
    timeLeft = roundTimes[0];
    return true;
}

void Game::spawnItems(int level) {
    golds.clear();
    int bigGoldCount, miniGoldCount, bombCount, mysteryBoxCount;

    if (level == 1) {
        bigGoldCount = 7; miniGoldCount = 5; bombCount = 4; mysteryBoxCount = 2;
    } else if (level == 2) {
        bigGoldCount = 5; miniGoldCount = 7; bombCount = 5; mysteryBoxCount = 2;
    } else {
        bigGoldCount = 4; miniGoldCount = 8; bombCount = 7; mysteryBoxCount = 1;
    }

    // Spawn big golds
    for (int i = 0; i < bigGoldCount; ++i) {
        Gold g;
        g.rect = { rand() % 600 + 100, rand() % 200 + 300, 50, 50 };
        g.isBomb = false;
        g.isPickedUp = false;
        g.isMysteryBox = false;
        g.value = 175;
        golds.push_back( g);
    }

    // Spawn mini golds
    for (int i = 0; i < miniGoldCount; ++i) {
        Gold g;
        g.rect = { rand() % 600 + 100, rand() % 200 + 300, 50, 50 };
        g.isBomb = false;
        g.isPickedUp = false;
        g.isMysteryBox = false;
        g.value = 100;
        golds.push_back(g);
    }

    // Spawn bombs
    for (int i = 0; i < bombCount; ++i) {
        Gold g;
        g.rect = { rand() % 600 + 100, rand() % 200 + 300, 50, 50 };
        g.isBomb = true;
        g.isPickedUp = false;
        g.isMysteryBox = false;
        g.value = 0;
        golds.push_back(g);
    }

    // Spawn mystery boxes
    for (int i = 0; i < mysteryBoxCount; ++i) {
        Gold g;
        g.rect = { rand() % 600 + 100, rand() % 200 + 300, 50, 50 };
        g.isBomb = false;
        g.isPickedUp = false;
        g.isMysteryBox = true;
        g.value = 0;
        golds.push_back(g);
    }
}

void Game::update() {
    elapsedTime = (SDL_GetTicks() - startTime) / 1000;
    timeLeft = roundTimes[currentRound] - elapsedTime;

    if (timeLeft <= 0) {
        if (score >= roundScores[currentRound]) {
            currentRound++;
            if (currentRound >= 3) {
                gameOver = true;
            } else {
                spawnItems(currentRound + 1);
                startTime = SDL_GetTicks();
                timeLeft = roundTimes[currentRound];
            }
        } else {
            gameOver = true;
        }
    }
}

void Game::render() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    for (const auto& gold : golds) {
        if (!gold.isPickedUp) {
            if (gold.isBomb) {
                SDL_RenderCopy(renderer, bombTexture, NULL, &gold.rect);
            } else if (gold.isMysteryBox) {
                SDL_RenderCopy(renderer, mysteryBoxTexture, NULL, &gold.rect);
            } else if (gold.value == 100) {
                SDL_RenderCopy(renderer, miniGoldTexture, NULL, &gold.rect);
            } else {
                SDL_RenderCopy(renderer, bigGoldTexture, NULL, &gold.rect);
            }
        }
    }

    string scoreText = "Score: " + to_string(score);
    SDL_Color textColor = { 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect renderQuad = { 10, 10, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    string timeText = "Time: " + to_string(timeLeft);
    textSurface = TTF_RenderText_Solid(font, timeText.c_str(), textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    renderQuad = { 600, 10, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    SDL_RenderPresent(renderer);
}