#ifndef MAINMENU_H
#define MAINMENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>

#define SCREEN_WIDTH  1920
#define SCREEN_HEIGHT 1080
#define MAX_BUTTONS   5

/* ── Structures ─────────────────────────────────────────────── */

typedef struct {
    SDL_Rect     rect;
    SDL_Texture *tex_normal;
    SDL_Texture *tex_hover;
    bool         hovered;
    bool         was_hovered;   /* to trigger hover sound once */
} Button;

typedef struct {
    /* SDL core */
    SDL_Window   *window;
    SDL_Renderer *renderer;

    /* Fonts */
    TTF_Font *font;        /* 32 pt – buttons / labels      */
    TTF_Font *titleFont;   /* 64 pt – game title             */

    /* Textures */
    SDL_Texture *background;   /* arrière-plan 1              */
    SDL_Texture *logo;         /* logo image                  */

    /* Buttons : Jouer, Options, Meilleurs Scores, Histoire, Quitter */
    Button buttons[MAX_BUTTONS];

    /* Audio */
    Mix_Music *bgMusic;
    Mix_Chunk *hoverSound;

    /* State */
    bool running;
    int  action;   /* 0=none 1=jouer 2=options 3=scores 4=histoire 5=quitter */
} MainMenu;

/* ── Function declarations ──────────────────────────────────── */

bool         MainMenu_Init(MainMenu *menu);
void         MainMenu_HandleEvents(MainMenu *menu);
void         MainMenu_Render(MainMenu *menu);
void         MainMenu_Cleanup(MainMenu *menu);
int          MainMenu_Run(MainMenu *menu);

/* Helpers */
SDL_Texture *LoadTexture(SDL_Renderer *renderer, const char *path);
SDL_Texture *RenderText(SDL_Renderer *renderer, TTF_Font *font,
                        const char *text, SDL_Color color);
bool         PointInRect(int x, int y, const SDL_Rect *r);

#endif /* MAINMENU_H */
