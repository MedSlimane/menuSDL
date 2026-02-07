prog:main.o scoremenu.o
	gcc -o prog main.o scoremenu.o -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
main.o: main.c
	gcc -c main.c -I/usr/include/SDL2
scoremenu.o: scoremenu.c
	gcc -c scoremenu.c -I/usr/include/SDL2

