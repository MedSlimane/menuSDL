prog:main.o engimemenu.o
	gcc -o prog main.o engimemenu.o -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
main.o: main.c
	gcc -c main.c -I/usr/include/SDL2
engimemenu.o: engimemenu.c
	gcc -c engimemenu.c -I/usr/include/SDL2

