prog:main.o optionmenu.o
	gcc -o prog main.o optionmenu.o -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
main.o: main.c
	gcc -c main.c -I/usr/include/SDL2
optionmenu.o: optionmenu.c
	gcc -c optionmenu.c -I/usr/include/SDL2

