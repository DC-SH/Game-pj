// #include "player.h"
// #include "intro.h"
// #include "game.h"
// #include "end.h"
// #include <SDL2/SDL.h>

// using namespace std;

// int main(int argc, char* args[]) {
//     Game game;
//     if (!game.init()) {
//         return -1;
//     }

//     Intro intro(game.getRenderer());
//     Player player(400, 200, game.getRenderer());
//     bool quit = false;
//     SDL_Event e;

//     while (!quit && !intro.isGameStarted()) {
//         while (SDL_PollEvent(&e) != 0) {
//             if (e.type == SDL_QUIT) quit = true;
//             intro.handleEvents(e);
//         }
//         intro.render();
//     }

//     while (!quit && !game.isGameOver()) {
//         while (SDL_PollEvent(&e) != 0) {
//             if (e.type == SDL_QUIT) quit = true;
//             if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
//                 player.attemptPickup(game.getGolds(), game.getScore(), game.setGameOver(true), game.getCurrentRound() + 1, game.setTimeLeft(game.getTimeLeft()));
//             }
//         }

//         player.updateSwing();
//         game.update();
//         game.render();
//         player.render();
//     }

//     End end(game.getRenderer(), TTF_OpenFont("arial.ttf", 24), game.getScore() >= 4700);
//     end.render();

//     return 0;
// }


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

    while (!quit && !intro.isGameStarted()) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            intro.handleEvents(e);
        }
        intro.render();
    }

    while (!quit && !game.isGameOver()) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                // Tạo các biến tạm để lưu giá trị
                int score = game.getScore();
                int timeLeft = game.getTimeLeft();
                bool gameOver = false;

                // Truyền các biến tạm vào hàm attemptPickup
                player.attemptPickup(game.getGolds(), score, gameOver, game.getCurrentRound() + 1, timeLeft);

                // Cập nhật lại giá trị cho game
                game.setGameOver(gameOver);
                game.setTimeLeft(timeLeft);
            }
        }

        player.updateSwing();
        game.update();
        game.render();
        player.render();
    }

    End end(game.getRenderer(), TTF_OpenFont("arial.ttf", 24), game.getScore() >= 4700);
    end.render();

    return 0;
}