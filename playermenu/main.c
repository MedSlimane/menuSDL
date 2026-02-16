/*  main.c – Player‑Menu standalone entry point */

#include <stdio.h>

#include "playermenu.h"

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    PlayerMenu menu;
    if (!PlayerMenu_Init(&menu)) {
        fprintf(stderr, "Failed to initialise Player Menu.\n");
        PlayerMenu_Cleanup(&menu);
        return 1;
    }

    int action;
    while (menu.running) {
        PlayerMenu_HandleEvents(&menu);
        PlayerMenu_Render(&menu);
        SDL_Delay(16);
    }
    action = menu.action;
    printf("Player menu action: %d  mode=%d avatar=%d input=%d\n", action,
           menu.playerMode, menu.selectedAvatar, menu.selectedInput);

    PlayerMenu_Cleanup(&menu);
    return 0;
}
