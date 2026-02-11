#ifndef PLAYERMENU_H
#define PLAYERMENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

typedef struct {
    SDL_Rect rect;
    SDL_Texture* tex_normal;
    SDL_Texture* tex_hover;
    int hovered;
    int was_hovered;
} Button;

typedef enum {
    PLAYER_VIEW_MODE,  /* Mono joueur / Multi joueurs / Retour */
    PLAYER_VIEW_CONFIG /* Avatar1/2  Input1/2  Valider  Retour */
} PlayerView;

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    TTF_Font* titleFont;
    SDL_Texture* background;

    /* View 1 */
    Button btnMono;
    Button btnMulti;
    Button btnRetour1;

    /* View 2 */
    Button btnAvatar1;
    Button btnAvatar2;
    Button btnInput1;
    Button btnInput2;
    Button btnValider;
    Button btnRetour2;

    int selectedAvatar; /* 1 or 2 */
    int selectedInput;  /* 1 or 2 */
    int playerMode;     /* 1=mono  2=multi */

    Mix_Music* bgMusic;
    Mix_Chunk* hoverSound;

    PlayerView view;
    int running;
    int action; /* 0=none 1=validated 2=retour */
} PlayerMenu;

int PlayerMenu_Init(PlayerMenu* menu);
void PlayerMenu_HandleEvents(PlayerMenu* menu);
void PlayerMenu_Render(PlayerMenu* menu);
void PlayerMenu_Cleanup(PlayerMenu* menu);
int PlayerMenu_Run(PlayerMenu* menu);

#endif
