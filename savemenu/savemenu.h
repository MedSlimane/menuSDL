#ifndef SAVEMENU_H
#define SAVEMENU_H

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

typedef enum {
    SAVE_VIEW_ASK,      /* "Voulez-vous sauvegarder ?"  OUI / NON   */
    SAVE_VIEW_LOAD      /* Charger le jeu / Nouvelle Partie          */
} SaveView;

typedef struct {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    TTF_Font     *font;
    TTF_Font     *titleFont;
    SDL_Texture  *background;    /* arrière-plan 2 */

    /* View 1 – Ask */
    Button btnOui;
    Button btnNon;

    /* View 2 – Load / New */
    Button btnCharger;
    Button btnNouvelle;

    Mix_Music *bgMusic;
    Mix_Chunk *hoverSound;

    SaveView view;
    bool     running;
    int      action;   /* 0=none 1=charger 2=nouvelle_partie 3=non(back) */
} SaveMenu;

/* ── Function declarations ──────────────────────────────────── */

bool         SaveMenu_Init(SaveMenu *menu);
void         SaveMenu_HandleEvents(SaveMenu *menu);
void         SaveMenu_Render(SaveMenu *menu);
void         SaveMenu_Cleanup(SaveMenu *menu);
int          SaveMenu_Run(SaveMenu *menu);

SDL_Texture *SM_LoadTexture(SDL_Renderer *r, const char *path);
SDL_Texture *SM_RenderText(SDL_Renderer *r, TTF_Font *f,
                           const char *text, SDL_Color c);
bool         SM_PointInRect(int x, int y, const SDL_Rect *r);

#endif /* SAVEMENU_H */
