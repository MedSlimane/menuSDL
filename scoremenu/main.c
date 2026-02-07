/*  main.c – Score‑Menu standalone entry point */

#include "scoremenu.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    (void)argc; (void)argv;

    ScoreMenu menu;
    if (!ScoreMenu_Init(&menu)) {
        fprintf(stderr, "Failed to initialise Score Menu.\n");
        ScoreMenu_Cleanup(&menu);
        return 1;
    }

    int action = ScoreMenu_Run(&menu);
    const char *labels[] = {"None", "Retour", "Quitter", "Enigme"};
    if (action >= 0 && action <= 3)
        printf("Score menu action: %s (%d)  player=%s\n",
               labels[action], action, menu.inputText);

    ScoreMenu_Cleanup(&menu);
    return 0;
}
