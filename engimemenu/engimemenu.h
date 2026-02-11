#ifndef ENGIMEMENU_H
#define ENGIMEMENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#define SCREEN_WIDTH  1920
#define SCREEN_HEIGHT 1080

typedef struct {
    SDL_Rect     rect;
    SDL_Texture *tex_normal;
    SDL_Texture *tex_hover;
    int          hovered;
    int          was_hovered;
} Button;

typedef enum {
    ENIGME_VIEW_CHOICE,   /* Quiz / Puzzle buttons               */
    ENIGME_VIEW_QUIZ      /* Question + A / B / C answer buttons */
} EnigmeView;

typedef struct {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    TTF_Font     *font;
    TTF_Font     *titleFont;
    SDL_Texture  *background;

    /* View 1 */
    Button btnQuiz;
    Button btnPuzzle;

    /* View 2 */
    Button btnA;
    Button btnB;
    Button btnC;

    /* Quiz data */
    const char *question;
    const char *answers[3];
    int         correctIndex;     /* 0 / 1 / 2 */
    int         selectedAnswer;   /* -1 = none  */

    Mix_Music *quizMusic;
    Mix_Chunk *hoverSound;

    EnigmeView view;
    int        running;
    int        action;   /* 0=none 1=answered 2=quit */
} EnigmeMenu;

int  EnigmeMenu_Init(EnigmeMenu *menu);
void EnigmeMenu_HandleEvents(EnigmeMenu *menu);
void EnigmeMenu_Render(EnigmeMenu *menu);
void EnigmeMenu_Cleanup(EnigmeMenu *menu);
int  EnigmeMenu_Run(EnigmeMenu *menu);

#endif
