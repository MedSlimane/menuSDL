/*  main.c – Save‑Menu standalone entry point */

#include <stdio.h>

#include "savemenu.h"

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    SaveMenu menu;
    if (!SaveMenu_Init(&menu)) {
        fprintf(stderr, "Failed to initialise Save Menu.\n");
        SaveMenu_Cleanup(&menu);
        return 1;
    }

    int action;
    while (menu.running) {
        SaveMenu_HandleEvents(&menu);
        SaveMenu_Render(&menu);
        SDL_Delay(16);
    }
    action = menu.action;
    const char* labels[] = {"None", "Charger le jeu", "Nouvelle Partie",
                            "Retour (Non)"};
    if (action >= 0 && action <= 3)
        printf("Save menu action: %s (%d)\n", labels[action], action);

    SaveMenu_Cleanup(&menu);
    return 0;
}
