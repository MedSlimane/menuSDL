prog:main.o playermenu.o
	gcc -o prog main.o playermenu.o -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
main.o: main.c
	gcc -c main.c -I/usr/include/SDL2
playermenu.o: playermenu.c
	gcc -c playermenu.c -I/usr/include/SDL2

