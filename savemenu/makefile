prog:main.o savemenu.o
	gcc -o prog main.o savemenu.o -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
main.o: main.c
	gcc -c main.c -I/usr/include/SDL2
savemenu.o: savemenu.c
	gcc -c savemenu.c -I/usr/include/SDL2

