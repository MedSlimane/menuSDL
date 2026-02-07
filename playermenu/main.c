/*  main.c – Player‑Menu standalone entry point */

#include "playermenu.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    (void)argc; (void)argv;

    PlayerMenu menu;
    if (!PlayerMenu_Init(&menu)) {
        fprintf(stderr, "Failed to initialise Player Menu.\n");
        PlayerMenu_Cleanup(&menu);
        return 1;
    }

    int action = PlayerMenu_Run(&menu);
    printf("Player menu action: %d  mode=%d avatar=%d input=%d\n",
           action, menu.playerMode, menu.selectedAvatar, menu.selectedInput);

    PlayerMenu_Cleanup(&menu);
    return 0;
}
