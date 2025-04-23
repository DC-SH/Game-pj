#include "player.h"
#include "intro.h"
#include "game.h"
#include "end.h"
#include <SDL2/SDL.h>

using namespace std;

int main(int argc, char* args[]) {
    Game game;
    if (!game.init()) {
        return -1;
    }

    Intro intro(game.getRenderer());
    Player player(400, 200, game.getRenderer());
    bool quit = false;
    SDL_Event e;

    // Khôi phục vòng lặp màn hình giới thiệu
    while (!quit && !intro.isGameStarted()) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            intro.handleEvents(e);
        }
        intro.render();
        SDL_Delay(33);
    }

    const int FPS = 30;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    while (!quit && !game.isGameOver()) {
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                int score = game.getScore();
                int timeLeft = game.getTimeLeft();
                bool gameOver = false;
                player.attemptPickup(game.getGolds(), score, gameOver, game.getCurrentRound() + 1, timeLeft);
                game.setGameOver(gameOver);
                game.setTimeLeft(timeLeft);
            }
        }

        player.updateSwing();
        game.update();
        game.render();
        player.render();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    End end(game.getRenderer(), TTF_OpenFont("arial.ttf", 24), game.getScore() >= 4700);
    end.render();

    SDL_Delay(2000);

    return 0;
}