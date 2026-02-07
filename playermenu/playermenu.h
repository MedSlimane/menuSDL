#ifndef PLAYERMENU_H
#define PLAYERMENU_H

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
    PLAYER_VIEW_MODE,     /* Mono joueur / Multi joueurs / Retour     */
    PLAYER_VIEW_CONFIG    /* Avatar1/2  Input1/2  Valider  Retour     */
} PlayerView;

typedef struct {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    TTF_Font     *font;
    TTF_Font     *titleFont;
    SDL_Texture  *background;    /* arrière-plan 2 */

    /* View 1 – mode selection */
    Button btnMono;
    Button btnMulti;
    Button btnRetour1;

    /* View 2 – avatar / input config */
    Button btnAvatar1;
    Button btnAvatar2;
    Button btnInput1;
    Button btnInput2;
    Button btnValider;
    Button btnRetour2;

    /* Selection tracking */
    int selectedAvatar;   /* 1 or 2 */
    int selectedInput;    /* 1 or 2 */
    int playerMode;       /* 1=mono  2=multi */

    Mix_Music *bgMusic;
    Mix_Chunk *hoverSound;

    PlayerView view;
    bool       running;
    int        action;    /* 0=none 1=validated 2=retour */
} PlayerMenu;

/* ── Function declarations ──────────────────────────────────── */

bool         PlayerMenu_Init(PlayerMenu *menu);
void         PlayerMenu_HandleEvents(PlayerMenu *menu);
void         PlayerMenu_Render(PlayerMenu *menu);
void         PlayerMenu_Cleanup(PlayerMenu *menu);
int          PlayerMenu_Run(PlayerMenu *menu);

SDL_Texture *PM_LoadTexture(SDL_Renderer *r, const char *path);
SDL_Texture *PM_RenderText(SDL_Renderer *r, TTF_Font *f,
                           const char *text, SDL_Color c);
bool         PM_PointInRect(int x, int y, const SDL_Rect *r);

#endif /* PLAYERMENU_H */
