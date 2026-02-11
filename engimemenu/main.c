/*  main.c – Enigme‑Menu standalone entry point */

#include "engimemenu.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    (void)argc; (void)argv;

    EnigmeMenu menu;
    if (!EnigmeMenu_Init(&menu)) {
        fprintf(stderr, "Failed to initialise Enigme Menu.\n");
        EnigmeMenu_Cleanup(&menu);
        return 1;
    }

    int action = EnigmeMenu_Run(&menu);

    if (action == 1) {
        int correct = (menu.selectedAnswer == menu.correctIndex);
        printf("Answer selected: %d (%s) – %s\n",
               menu.selectedAnswer,
               menu.answers[menu.selectedAnswer],
               correct ? "CORRECT!" : "WRONG");
    } else {
        printf("Enigme menu exited (action=%d)\n", action);
    }

    EnigmeMenu_Cleanup(&menu);
    return 0;
}
