#ifndef MAINMENU_H
#define MAINMENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define MAX_BUTTONS 5

typedef struct {
    SDL_Rect rect;
    SDL_Texture* tex_normal;
    SDL_Texture* tex_hover;
    int hovered;
    int was_hovered;
} Button;

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    TTF_Font* titleFont;
    SDL_Texture* background;
    SDL_Texture* logo;
    Button buttons[MAX_BUTTONS];
    Mix_Music* bgMusic;
    Mix_Chunk* hoverSound;
    int running;
    int action; /* 0=none 1=jouer 2=options 3=scores 4=histoire 5=quitter */
} MainMenu;

int MainMenu_Init(MainMenu* menu);
void MainMenu_HandleEvents(MainMenu* menu);
void MainMenu_Render(MainMenu* menu);
void MainMenu_Cleanup(MainMenu* menu);
int MainMenu_Run(MainMenu* menu);

#endif
