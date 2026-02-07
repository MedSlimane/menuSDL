/*  main.c – Save‑Menu standalone entry point */

#include "savemenu.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    (void)argc; (void)argv;

    SaveMenu menu;
    if (!SaveMenu_Init(&menu)) {
        fprintf(stderr, "Failed to initialise Save Menu.\n");
        SaveMenu_Cleanup(&menu);
        return 1;
    }

    int action = SaveMenu_Run(&menu);
    const char *labels[] = {"None", "Charger le jeu", "Nouvelle Partie", "Retour (Non)"};
    if (action >= 0 && action <= 3)
        printf("Save menu action: %s (%d)\n", labels[action], action);

    SaveMenu_Cleanup(&menu);
    return 0;
}
