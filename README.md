# Gold Miner Game

**Author:** Hoàng Sỹ Hùng K69_I_IS4

A classic gold mining game built with C++, SDL2, SDL2_image, SDL2_ttf, and SDL2_mixer.

## Features

- Multiple rounds with increasing difficulty
- Various collectible items: big gold, mini gold, bombs, mystery boxes
- Animated explosions and sound effects
- Score and time tracking
- Win/lose end screens

## Controls

- **Space** or **Mouse Click**: Attempt to pick up items
- **ESC** or close window: Quit game

## Requirements

- SDL2
- SDL2_image
- SDL2_ttf
- SDL2_mixer

## How to Build

1. Install SDL2 and related libraries.
2. Run:
    ```sh
    make
    ```
   or compile manually using the provided [Makefile](http://_vscodecontentref_/0).

## How to Play

1. Launch the game:
    ```sh
    ./main.exe
    ```
2. Start the game from the intro screen.
3. Use the controls to collect gold and avoid bombs.
4. Reach the target score before time runs out to advance rounds.

## File Structure

- [main.cpp](http://_vscodecontentref_/1) - Entry point
- `Game.cpp/h` - Game logic and rendering
- `Character.cpp/h`, `Item.cpp/h` - Game entities
- `end.cpp` - End screen
- `intro.h` - Intro screen
- `assets/` - Images, sounds, and fonts

## Assets

- Images: [b1.png](http://_vscodecontentref_/2), [b2.png](http://_vscodecontentref_/3), [back1.png](http://_vscodecontentref_/4), [back2.png](http://_vscodecontentref_/5), [back3.png](http://_vscodecontentref_/6), [biggold.png](http://_vscodecontentref_/7), [bomb.png](http://_vscodecontentref_/8), [minigold.png](http://_vscodecontentref_/9), [mysbox.png](http://_vscodecontentref_/10), [players.png](http://_vscodecontentref_/11), [chill.png](http://_vscodecontentref_/12), [Explode.png](http://_vscodecontentref_/13)
- Sounds: `bomb explode.mp3`, `GOLD OR MYSBOX.mp3`, `Losing sound.mp3`, `SPACE OR CLICK.mp3`, `Winning sound.mp3`
- Font: [font.ttf](http://_vscodecontentref_/14)

## Credits

- Assets from open sources or self-made

## License

This project is for educational purposes.