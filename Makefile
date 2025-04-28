all:
	g++ -I src/include -L src/lib -o main main.cpp Game.cpp Character.cpp Item.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

run:
	.\main
