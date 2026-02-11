#ifndef SAVEMENU_H
#define SAVEMENU_H

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
    SAVE_VIEW_ASK, /* Voulez-vous sauvegarder ?  OUI / NON */
    SAVE_VIEW_LOAD /* Charger le jeu / Nouvelle Partie      */
} SaveView;

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    TTF_Font* titleFont;
    SDL_Texture* background;
    Button btnOui;
    Button btnNon;
    Button btnCharger;
    Button btnNouvelle;
    Mix_Music* bgMusic;
    Mix_Chunk* hoverSound;
    SaveView view;
    int running;
    int action; /* 0=none 1=charger 2=nouvelle_partie 3=non(back) */
} SaveMenu;

int SaveMenu_Init(SaveMenu* menu);
void SaveMenu_HandleEvents(SaveMenu* menu);
void SaveMenu_Render(SaveMenu* menu);
void SaveMenu_Cleanup(SaveMenu* menu);
int SaveMenu_Run(SaveMenu* menu);

#endif
