#ifndef OPTIONMENU_H
#define OPTIONMENU_H

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

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    TTF_Font* labelFont;
    SDL_Texture* background;
    Button btnDiminuer;
    Button btnAugmenter;
    Button btnNormal;
    Button btnPleinEcran;
    Button btnRetour;
    Mix_Music* bgMusic;
    Mix_Chunk* hoverSound;
    int volume; /* 0 – 128 */
    int fullscreen;
    int running;
    int action; /* 0=none 1=retour */
} OptionMenu;

int OptionMenu_Init(OptionMenu* menu);
void OptionMenu_HandleEvents(OptionMenu* menu);
void OptionMenu_Render(OptionMenu* menu);
void OptionMenu_Cleanup(OptionMenu* menu);
int OptionMenu_Run(OptionMenu* menu);

#endif
