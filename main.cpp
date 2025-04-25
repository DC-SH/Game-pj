#include "player.h"
#include "intro.h"
#include "game.h"
#include "end.h"
#include <SDL2/SDL.h>
#include<iostream>

using namespace std;

int main(int argc, char* args[]) {
    Game game;
    if (!game.init()) {
        cerr << "Game initialization failed!" << endl;
        return -1;
    }

    Intro intro(game.getRenderer());
    Player player(400, 200, game.getRenderer());
    bool quit = false;
    SDL_Event e;
    bool gameStarted = false;

    intro.render(); // Hiển thị màn hình giới thiệu

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
                break;
            }
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_SPACE) {
                    if (!gameStarted) {
                        gameStarted = true; // Bắt đầu trò chơi
                        game.render();
                        player.render();
                    } else if (!game.isGameOver() && !game.isGameWon() && !game.isPaused()) {
                        player.updateSwing(); // Cập nhật xoay
                        int score = game.getScore();
                        bool gameOver = false;
                        player.attemptPickup(game.getGolds(), score, gameOver, game.getCurrentRound());
                        game.setGameOver(gameOver);
                        game.update();
                        game.render();
                        player.render();
                    }
                } else if (e.key.keysym.sym == SDLK_r && gameStarted && !game.isGameOver() && !game.isGameWon()) {
                    game.togglePause(); // Tạm dừng/tiếp tục
                    if (game.isPaused()) {
                        SDL_Surface* pauseSurface = TTF_OpenFont("arial.ttf", 24) ? TTF_RenderText_Solid(TTF_OpenFont("arial.ttf", 24), "PAUSED", { 255, 255, 255 }) : nullptr;
                        if (pauseSurface) {
                            SDL_Texture* pauseTexture = SDL_CreateTextureFromSurface(game.getRenderer(), pauseSurface);
                            SDL_Rect pauseRect = { 350, 300, pauseSurface->w, pauseSurface->h };
                            SDL_RenderCopy(game.getRenderer(), pauseTexture, NULL, &pauseRect);
                            SDL_RenderPresent(game.getRenderer());
                            SDL_DestroyTexture(pauseTexture);
                            SDL_FreeSurface(pauseSurface);
                        }
                    } else {
                        game.render();
                        player.render();
                    }
                }
            }
        }
    }

    if (game.isGameOver() || game.isGameWon()) {
        End end(game.getRenderer(), TTF_OpenFont("arial.ttf", 24), game.isGameWon());
        end.render();
        SDL_Delay(2000);
    }

    return 0;
}