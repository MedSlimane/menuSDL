/*  main.c – Main‑Menu standalone entry point */

#include <stdio.h>

#include "mainmenu.h"

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    MainMenu menu;
    if (!MainMenu_Init(&menu)) {
        fprintf(stderr, "Failed to initialise Main Menu.\n");
        MainMenu_Cleanup(&menu);
        return 1;
    }

    int action;
    while (menu.running) {
        MainMenu_HandleEvents(&menu);
        MainMenu_Render(&menu);
        SDL_Delay(16);
    }
    action = menu.action;

    const char* labels[] = {"None",     "Jouer",  "Options", "Meilleurs Scores",
                            "Histoire", "Quitter"};
    if (action >= 1 && action <= 5)
        printf("Action selected: %s (%d)\n", labels[action], action);

    MainMenu_Cleanup(&menu);
    return 0;
}
