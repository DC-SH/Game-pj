all:
	g++ -I ../src/Include -L ../src/lib -o main main.cpp game.cpp player.cpp intro.cpp end.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
run:
	./main