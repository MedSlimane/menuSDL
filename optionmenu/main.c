/*  main.c – Option‑Menu standalone entry point */

#include <stdio.h>

#include "optionmenu.h"

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    OptionMenu menu;
    if (!OptionMenu_Init(&menu)) {
        fprintf(stderr, "Failed to initialise Option Menu.\n");
        OptionMenu_Cleanup(&menu);
        return 1;
    }

    int action;
    while (menu.running) {
        OptionMenu_HandleEvents(&menu);
        OptionMenu_Render(&menu);
        SDL_Delay(16);
    }
    action = menu.action;
    printf("Option menu exited with action: %d\n", action);

    OptionMenu_Cleanup(&menu);
    return 0;
}
