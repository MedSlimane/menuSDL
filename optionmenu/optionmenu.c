/*  optionmenu.c – Options sub‑menu implementation (SDL2)
 *  Volume : Diminuer / Augmenter   (+/- keys)
 *  Display: Normal / Plein écran
 *  Retour button returns to main menu
 */

#include "optionmenu.h"
#include <stdio.h>
#include <string.h>

/* ── Helpers ────────────────────────────────────────────────── */

SDL_Texture *OM_LoadTexture(SDL_Renderer *r, const char *path)
{
    SDL_Surface *s = IMG_Load(path);
    if (!s) { fprintf(stderr, "IMG_Load(%s): %s\n", path, IMG_GetError()); return NULL; }
    SDL_Texture *t = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);
    return t;
}

SDL_Texture *OM_RenderText(SDL_Renderer *r, TTF_Font *f,
                           const char *text, SDL_Color c)
{
    SDL_Surface *s = TTF_RenderUTF8_Blended(f, text, c);
    if (!s) return NULL;
    SDL_Texture *t = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);
    return t;
}

bool OM_PointInRect(int x, int y, const SDL_Rect *r)
{
    return x >= r->x && x < r->x + r->w &&
           y >= r->y && y < r->y + r->h;
}

static Button MakeBtn(SDL_Renderer *r, int x, int y, int w, int h,
                      const char *n, const char *hv)
{
    Button b;
    memset(&b, 0, sizeof(b));
    b.rect = (SDL_Rect){x, y, w, h};
    b.tex_normal = OM_LoadTexture(r, n);
    b.tex_hover  = OM_LoadTexture(r, hv);
    return b;
}

static void UpdateHover(Button *b, int mx, int my, Mix_Chunk *snd)
{
    b->hovered = OM_PointInRect(mx, my, &b->rect);
    if (b->hovered && !b->was_hovered && snd) Mix_PlayChannel(-1, snd, 0);
    b->was_hovered = b->hovered;
}

static void DrawButton(SDL_Renderer *r, Button *b)
{
    SDL_Texture *tex = b->hovered ? b->tex_hover : b->tex_normal;
    if (tex) SDL_RenderCopy(r, tex, NULL, &b->rect);
    else {
        SDL_SetRenderDrawColor(r, b->hovered?200:100, b->hovered?200:100,
                               b->hovered?200:100, 255);
        SDL_RenderFillRect(r, &b->rect);
        SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
        SDL_RenderDrawRect(r, &b->rect);
    }
}

static void DrawLabel(SDL_Renderer *r, TTF_Font *f, const char *txt,
                      SDL_Rect *btn)
{
    if (!f) return;
    SDL_Color black = {0,0,0,255};
    SDL_Texture *t = OM_RenderText(r, f, txt, black);
    if (!t) return;
    int tw, th; SDL_QueryTexture(t, NULL, NULL, &tw, &th);
    SDL_Rect dst = {btn->x + (btn->w-tw)/2, btn->y + (btn->h-th)/2, tw, th};
    SDL_RenderCopy(r, t, NULL, &dst);
    SDL_DestroyTexture(t);
}

/* ── Init ───────────────────────────────────────────────────── */

bool OptionMenu_Init(OptionMenu *menu)
{
    memset(menu, 0, sizeof(*menu));

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return false;
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))       return false;
    if (TTF_Init() < 0)                                 return false;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) return false;

    menu->window = SDL_CreateWindow("Sous Menu Option",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!menu->window) return false;

    menu->renderer = SDL_CreateRenderer(menu->window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!menu->renderer) return false;

    menu->font      = TTF_OpenFont("assets/font.ttf", 32);
    menu->labelFont = TTF_OpenFont("assets/font.ttf", 40);

    menu->background = OM_LoadTexture(menu->renderer, "assets/background2.png");

    /* Volume row : centre‑ish, two buttons side by side */
    int row1Y = 300;
    menu->btnDiminuer  = MakeBtn(menu->renderer, 600,  row1Y, 200, 80,
                                 "assets/btn_diminuer.png",  "assets/btn_diminuer_h.png");
    menu->btnAugmenter = MakeBtn(menu->renderer, 900,  row1Y, 200, 80,
                                 "assets/btn_augmenter.png", "assets/btn_augmenter_h.png");

    /* Display row */
    int row2Y = 500;
    menu->btnNormal    = MakeBtn(menu->renderer, 600,  row2Y, 200, 80,
                                 "assets/btn_normal.png",    "assets/btn_normal_h.png");
    menu->btnPleinEcran= MakeBtn(menu->renderer, 900,  row2Y, 200, 80,
                                 "assets/btn_pleinecran.png","assets/btn_pleinecran_h.png");

    /* Retour */
    menu->btnRetour = MakeBtn(menu->renderer, SCREEN_WIDTH - 350, SCREEN_HEIGHT - 150,
                              200, 80,
                              "assets/btn_retour.png", "assets/btn_retour_h.png");

    menu->bgMusic    = Mix_LoadMUS("assets/music_option.mp3");
    menu->hoverSound = Mix_LoadWAV("assets/hover.wav");
    if (menu->bgMusic) Mix_PlayMusic(menu->bgMusic, -1);

    menu->volume     = MIX_MAX_VOLUME / 2;
    menu->fullscreen = false;
    menu->running    = true;
    menu->action     = 0;
    Mix_VolumeMusic(menu->volume);
    return true;
}

/* ── Events ─────────────────────────────────────────────────── */

void OptionMenu_HandleEvents(OptionMenu *menu)
{
    int mx, my; SDL_GetMouseState(&mx, &my);
    Button *btns[] = {&menu->btnDiminuer, &menu->btnAugmenter,
                      &menu->btnNormal, &menu->btnPleinEcran, &menu->btnRetour};
    for (int i = 0; i < 5; i++) UpdateHover(btns[i], mx, my, menu->hoverSound);

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) { menu->running = false; return; }

        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_PLUS: case SDLK_KP_PLUS:
                    menu->volume = SDL_min(menu->volume + 10, MIX_MAX_VOLUME);
                    Mix_VolumeMusic(menu->volume);
                    break;
                case SDLK_MINUS: case SDLK_KP_MINUS:
                    menu->volume = SDL_max(menu->volume - 10, 0);
                    Mix_VolumeMusic(menu->volume);
                    break;
                case SDLK_ESCAPE:
                    menu->action = 1; menu->running = false; break;
                default: break;
            }
        }

        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            int cx = e.button.x, cy = e.button.y;
            if (OM_PointInRect(cx, cy, &menu->btnDiminuer.rect)) {
                menu->volume = SDL_max(menu->volume - 10, 0);
                Mix_VolumeMusic(menu->volume);
            }
            if (OM_PointInRect(cx, cy, &menu->btnAugmenter.rect)) {
                menu->volume = SDL_min(menu->volume + 10, MIX_MAX_VOLUME);
                Mix_VolumeMusic(menu->volume);
            }
            if (OM_PointInRect(cx, cy, &menu->btnNormal.rect)) {
                SDL_SetWindowFullscreen(menu->window, 0);
                menu->fullscreen = false;
            }
            if (OM_PointInRect(cx, cy, &menu->btnPleinEcran.rect)) {
                SDL_SetWindowFullscreen(menu->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                menu->fullscreen = true;
            }
            if (OM_PointInRect(cx, cy, &menu->btnRetour.rect)) {
                menu->action = 1; menu->running = false;
            }
        }
    }
}

/* ── Render ─────────────────────────────────────────────────── */

void OptionMenu_Render(OptionMenu *menu)
{
    SDL_RenderClear(menu->renderer);
    if (menu->background) SDL_RenderCopy(menu->renderer, menu->background, NULL, NULL);

    /* Labels */
    if (menu->labelFont) {
        SDL_Color white = {255,255,255,255};
        SDL_Texture *vl = OM_RenderText(menu->renderer, menu->labelFont, "Volume", white);
        if (vl) {
            SDL_Rect r = {350, 310, 200, 60};
            SDL_RenderCopy(menu->renderer, vl, NULL, &r);
            SDL_DestroyTexture(vl);
        }
        SDL_Texture *dl = OM_RenderText(menu->renderer, menu->labelFont,
                                        "Mode d'affichage", white);
        if (dl) {
            SDL_Rect r = {250, 510, 320, 60};
            SDL_RenderCopy(menu->renderer, dl, NULL, &r);
            SDL_DestroyTexture(dl);
        }

        /* Volume value */
        char volTxt[32];
        snprintf(volTxt, sizeof(volTxt), "%d%%", menu->volume * 100 / MIX_MAX_VOLUME);
        SDL_Texture *vt = OM_RenderText(menu->renderer, menu->font, volTxt, white);
        if (vt) {
            SDL_Rect r = {1150, 310, 80, 40};
            SDL_RenderCopy(menu->renderer, vt, NULL, &r);
            SDL_DestroyTexture(vt);
        }
    }

    /* Buttons */
    DrawButton(menu->renderer, &menu->btnDiminuer);
    DrawButton(menu->renderer, &menu->btnAugmenter);
    DrawButton(menu->renderer, &menu->btnNormal);
    DrawButton(menu->renderer, &menu->btnPleinEcran);
    DrawButton(menu->renderer, &menu->btnRetour);



    SDL_RenderPresent(menu->renderer);
}

/* ── Cleanup ────────────────────────────────────────────────── */

void OptionMenu_Cleanup(OptionMenu *menu)
{
    Button *btns[] = {&menu->btnDiminuer, &menu->btnAugmenter,
                      &menu->btnNormal, &menu->btnPleinEcran, &menu->btnRetour};
    for (int i = 0; i < 5; i++) {
        if (btns[i]->tex_normal) SDL_DestroyTexture(btns[i]->tex_normal);
        if (btns[i]->tex_hover)  SDL_DestroyTexture(btns[i]->tex_hover);
    }
    if (menu->background)  SDL_DestroyTexture(menu->background);
    if (menu->bgMusic)     Mix_FreeMusic(menu->bgMusic);
    if (menu->hoverSound)  Mix_FreeChunk(menu->hoverSound);
    if (menu->font)        TTF_CloseFont(menu->font);
    if (menu->labelFont)   TTF_CloseFont(menu->labelFont);
    if (menu->renderer)    SDL_DestroyRenderer(menu->renderer);
    if (menu->window)      SDL_DestroyWindow(menu->window);
    Mix_CloseAudio(); TTF_Quit(); IMG_Quit(); SDL_Quit();
}

/* ── Run ────────────────────────────────────────────────────── */

int OptionMenu_Run(OptionMenu *menu)
{
    while (menu->running) {
        OptionMenu_HandleEvents(menu);
        OptionMenu_Render(menu);
        SDL_Delay(16);
    }
    return menu->action;
}
