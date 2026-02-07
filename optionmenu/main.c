/*  main.c – Option‑Menu standalone entry point */

#include "optionmenu.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    (void)argc; (void)argv;

    OptionMenu menu;
    if (!OptionMenu_Init(&menu)) {
        fprintf(stderr, "Failed to initialise Option Menu.\n");
        OptionMenu_Cleanup(&menu);
        return 1;
    }

    int action = OptionMenu_Run(&menu);
    printf("Option menu exited with action: %d\n", action);

    OptionMenu_Cleanup(&menu);
    return 0;
}
