#ifndef ENGIMEMENU_H
#define ENGIMEMENU_H

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
    ENIGME_VIEW_CHOICE,   /* Quiz / Puzzle buttons                   */
    ENIGME_VIEW_QUIZ      /* Question + A / B / C answer buttons     */
} EnigmeView;

typedef struct {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    TTF_Font     *font;
    TTF_Font     *titleFont;
    SDL_Texture  *background;     /* arrière-plan 4 */

    /* View 1 – choice */
    Button btnQuiz;
    Button btnPuzzle;

    /* View 2 – quiz */
    Button btnA;
    Button btnB;
    Button btnC;

    /* Quiz data */
    const char *question;
    const char *answers[3];
    int         correctIndex;     /* 0 / 1 / 2 */
    int         selectedAnswer;   /* -1 = none  */

    Mix_Music *quizMusic;         /* suspense / drum roll */
    Mix_Chunk *hoverSound;

    EnigmeView view;
    bool       running;
    int        action;   /* 0=none 1=answered 2=quit */
} EnigmeMenu;

/* ── Function declarations ──────────────────────────────────── */

bool         EnigmeMenu_Init(EnigmeMenu *menu);
void         EnigmeMenu_HandleEvents(EnigmeMenu *menu);
void         EnigmeMenu_Render(EnigmeMenu *menu);
void         EnigmeMenu_Cleanup(EnigmeMenu *menu);
int          EnigmeMenu_Run(EnigmeMenu *menu);

SDL_Texture *EM_LoadTexture(SDL_Renderer *r, const char *path);
SDL_Texture *EM_RenderText(SDL_Renderer *r, TTF_Font *f,
                           const char *text, SDL_Color c);
bool         EM_PointInRect(int x, int y, const SDL_Rect *r);

#endif /* ENGIMEMENU_H */
