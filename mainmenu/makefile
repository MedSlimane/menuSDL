prog:main.o mainmenu.o
	gcc -o prog main.o mainmenu.o -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
main.o: main.c
	gcc -c main.c -I/usr/include/SDL2
mainmenu.o: mainmenu.c
	gcc -c mainmenu.c -I/usr/include/SDL2

