#ifndef SCOREMENU_H
#define SCOREMENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>

#define SCREEN_WIDTH  1920
#define SCREEN_HEIGHT 1080
#define MAX_NAME_LEN  32
#define TOP_SCORES    3

/* ── Structures ─────────────────────────────────────────────── */

typedef struct {
    SDL_Rect     rect;
    SDL_Texture *tex_normal;
    SDL_Texture *tex_hover;
    bool         hovered;
    bool         was_hovered;
} Button;

typedef struct {
    char name[MAX_NAME_LEN];
    int  score;
} ScoreEntry;

typedef enum {
    SCORE_VIEW_INPUT,    /* Name input + Valider                     */
    SCORE_VIEW_DISPLAY   /* Top 3 scores + Quitter / Retour          */
} ScoreView;

typedef struct {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    TTF_Font     *font;
    TTF_Font     *titleFont;
    SDL_Texture  *background;    /* arrière-plan 3 */

    /* View 1 – name input */
    char   inputText[MAX_NAME_LEN];
    int    inputLen;
    Button btnValider;

    /* View 2 – score display */
    ScoreEntry topScores[TOP_SCORES];
    SDL_Texture *starTextures[TOP_SCORES];  /* star1, star2, star3 */
    Button btnQuitter;
    Button btnRetour;

    Mix_Music *bgMusic;          /* victory music for score display */
    Mix_Chunk *hoverSound;

    ScoreView view;
    bool      running;
    int       action;    /* 0=none 1=retour 2=quitter 3=enigme */
} ScoreMenu;

/* ── Function declarations ──────────────────────────────────── */

bool         ScoreMenu_Init(ScoreMenu *menu);
void         ScoreMenu_HandleEvents(ScoreMenu *menu);
void         ScoreMenu_Render(ScoreMenu *menu);
void         ScoreMenu_Cleanup(ScoreMenu *menu);
int          ScoreMenu_Run(ScoreMenu *menu);

SDL_Texture *SCM_LoadTexture(SDL_Renderer *r, const char *path);
SDL_Texture *SCM_RenderText(SDL_Renderer *r, TTF_Font *f,
                            const char *text, SDL_Color c);
bool         SCM_PointInRect(int x, int y, const SDL_Rect *r);

#endif /* SCOREMENU_H */
