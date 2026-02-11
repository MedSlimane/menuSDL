#ifndef SCOREMENU_H
#define SCOREMENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#define SCREEN_WIDTH  1920
#define SCREEN_HEIGHT 1080
#define MAX_NAME_LEN  32
#define TOP_SCORES    3

typedef struct {
    SDL_Rect     rect;
    SDL_Texture *tex_normal;
    SDL_Texture *tex_hover;
    int          hovered;
    int          was_hovered;
} Button;

typedef struct {
    char name[MAX_NAME_LEN];
    int  score;
} ScoreEntry;

typedef enum {
    SCORE_VIEW_INPUT,    /* Name input + Valider          */
    SCORE_VIEW_DISPLAY   /* Top 3 scores + Quitter/Retour */
} ScoreView;

typedef struct {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    TTF_Font     *font;
    TTF_Font     *titleFont;
    SDL_Texture  *background;
    char          inputText[MAX_NAME_LEN];
    int           inputLen;
    Button        btnValider;
    ScoreEntry    topScores[TOP_SCORES];
    SDL_Texture  *starTextures[TOP_SCORES];
    Button        btnQuitter;
    Button        btnRetour;
    Mix_Music    *bgMusic;
    Mix_Chunk    *hoverSound;
    ScoreView     view;
    int           running;
    int           action;    /* 0=none 1=retour 2=quitter 3=enigme */
} ScoreMenu;

int  ScoreMenu_Init(ScoreMenu *menu);
void ScoreMenu_HandleEvents(ScoreMenu *menu);
void ScoreMenu_Render(ScoreMenu *menu);
void ScoreMenu_Cleanup(ScoreMenu *menu);
int  ScoreMenu_Run(ScoreMenu *menu);

#endif
