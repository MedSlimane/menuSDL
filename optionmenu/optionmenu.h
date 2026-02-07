#ifndef OPTIONMENU_H
#define OPTIONMENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>

#define SCREEN_WIDTH  1920
#define SCREEN_HEIGHT 1080

/* ── Structures ─────────────────────────────────────────────── */

typedef struct {
    SDL_Rect     rect;
    SDL_Texture *tex_normal;
    SDL_Texture *tex_hover;
    bool         hovered;
    bool         was_hovered;
} Button;

typedef struct {
    SDL_Window   *window;
    SDL_Renderer *renderer;

    TTF_Font *font;
    TTF_Font *labelFont;

    SDL_Texture *background;   /* arrière-plan 2 */

    /* Volume controls */
    Button btnDiminuer;
    Button btnAugmenter;

    /* Display mode */
    Button btnNormal;
    Button btnPleinEcran;

    /* Navigation */
    Button btnRetour;

    Mix_Music *bgMusic;
    Mix_Chunk *hoverSound;

    int  volume;          /* 0 – 128 */
    bool fullscreen;
    bool running;
    int  action;          /* 0=none 1=retour */
} OptionMenu;

/* ── Function declarations ──────────────────────────────────── */

bool         OptionMenu_Init(OptionMenu *menu);
void         OptionMenu_HandleEvents(OptionMenu *menu);
void         OptionMenu_Render(OptionMenu *menu);
void         OptionMenu_Cleanup(OptionMenu *menu);
int          OptionMenu_Run(OptionMenu *menu);

/* Helpers */
SDL_Texture *OM_LoadTexture(SDL_Renderer *r, const char *path);
SDL_Texture *OM_RenderText(SDL_Renderer *r, TTF_Font *f,
                           const char *text, SDL_Color c);
bool         OM_PointInRect(int x, int y, const SDL_Rect *r);

#endif /* OPTIONMENU_H */
