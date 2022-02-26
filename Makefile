#temporary makefile
all:
	g++ -I include -L lib -o main src/main.cpp src/chip8.cpp src/display.cpp -lmingw32 -lSDL2main -lSDL2 -static-libgcc -static-libstdc++
