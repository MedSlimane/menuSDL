/*  mainmenu.c – Main‑Menu implementation (SDL2)
 *  Buttons : Jouer · Options · Meilleurs Scores · Histoire · Quitter
 *  Keyboard: J=Jouer  O=Options  M=Scores  Escape=Quitter
 */

#include "mainmenu.h"
#include <stdio.h>
#include <string.h>

/* ── Helpers ────────────────────────────────────────────────── */

SDL_Texture *LoadTexture(SDL_Renderer *renderer, const char *path)
{
    SDL_Surface *s = IMG_Load(path);
    if (!s) { fprintf(stderr, "IMG_Load(%s): %s\n", path, IMG_GetError()); return NULL; }
    SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
    SDL_FreeSurface(s);
    return t;
}

SDL_Texture *RenderText(SDL_Renderer *renderer, TTF_Font *font,
                        const char *text, SDL_Color color)
{
    SDL_Surface *s = TTF_RenderUTF8_Blended(font, text, color);
    if (!s) return NULL;
    SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
    SDL_FreeSurface(s);
    return t;
}

bool PointInRect(int x, int y, const SDL_Rect *r)
{
    return x >= r->x && x <= r->x + r->w &&
           y >= r->y && y <= r->y + r->h;
}

/* ── Button creation helper ─────────────────────────────────── */

static Button MakeButton(SDL_Renderer *renderer,
                         int x, int y, int w, int h,
                         const char *normalImg, const char *hoverImg)
{
    Button b;
    memset(&b, 0, sizeof(b));
    b.rect = (SDL_Rect){x, y, w, h};
    b.tex_normal = LoadTexture(renderer, normalImg);
    b.tex_hover  = LoadTexture(renderer, hoverImg);
    b.hovered = false;
    b.was_hovered = false;
    return b;
}

/* ── Init ───────────────────────────────────────────────────── */

bool MainMenu_Init(MainMenu *menu)
{
    memset(menu, 0, sizeof(*menu));

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return false;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "IMG_Init: %s\n", IMG_GetError());
        return false;
    }
    if (TTF_Init() < 0) {
        fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
        return false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Mix_OpenAudio: %s\n", Mix_GetError());
        return false;
    }

    menu->window = SDL_CreateWindow("Menu Principal",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!menu->window) return false;

    menu->renderer = SDL_CreateRenderer(menu->window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!menu->renderer) return false;

    /* Fonts – replace path with your actual .ttf file */
    menu->font      = TTF_OpenFont("assets/font.ttf", 32);
    menu->titleFont = TTF_OpenFont("assets/font.ttf", 64);
    if (!menu->font)      fprintf(stderr, "font.ttf not found – text won't render\n");
    if (!menu->titleFont) fprintf(stderr, "titleFont not found – title won't render\n");

    /* Background & logo */
    menu->background = LoadTexture(menu->renderer, "assets/background1.png");
    menu->logo       = LoadTexture(menu->renderer, "assets/logo.png");

    /* Buttons – left column, stacked vertically */
    int bx = 150, by = 180, bw = 400, bh = 100, gap = 40;
    menu->buttons[0] = MakeButton(menu->renderer, bx, by + 0*(bh+gap), bw, bh,
                                  "assets/btn_jouer.png",    "assets/btn_jouer_h.png");
    menu->buttons[1] = MakeButton(menu->renderer, bx, by + 1*(bh+gap), bw, bh,
                                  "assets/btn_options.png",  "assets/btn_options_h.png");
    menu->buttons[2] = MakeButton(menu->renderer, bx, by + 2*(bh+gap), bw, bh,
                                  "assets/btn_scores.png",   "assets/btn_scores_h.png");
    menu->buttons[3] = MakeButton(menu->renderer, bx, by + 3*(bh+gap), bw, bh,
                                  "assets/btn_histoire.png", "assets/btn_histoire_h.png");
    menu->buttons[4] = MakeButton(menu->renderer, SCREEN_WIDTH - 400, SCREEN_HEIGHT - 150,
                                  200, 80,
                                  "assets/btn_quitter.png",  "assets/btn_quitter_h.png");

    /* Audio */
    menu->bgMusic   = Mix_LoadMUS("assets/music.mp3");
    menu->hoverSound = Mix_LoadWAV("assets/hover.wav");

    if (menu->bgMusic) Mix_PlayMusic(menu->bgMusic, -1);

    menu->running = true;
    menu->action  = 0;
    return true;
}

/* ── Events ─────────────────────────────────────────────────── */

void MainMenu_HandleEvents(MainMenu *menu)
{
    SDL_Event e;
    int mx, my;
    SDL_GetMouseState(&mx, &my);

    /* Update hover state */
    for (int i = 0; i < MAX_BUTTONS; i++) {
        menu->buttons[i].hovered = PointInRect(mx, my, &menu->buttons[i].rect);
        if (menu->buttons[i].hovered && !menu->buttons[i].was_hovered) {
            if (menu->hoverSound) Mix_PlayChannel(-1, menu->hoverSound, 0);
        }
        menu->buttons[i].was_hovered = menu->buttons[i].hovered;
    }

    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) { menu->running = false; menu->action = 5; }

        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_j:      menu->action = 1; menu->running = false; break;
                case SDLK_o:      menu->action = 2; menu->running = false; break;
                case SDLK_m:      menu->action = 3; menu->running = false; break;
                case SDLK_ESCAPE: menu->action = 5; menu->running = false; break;
                default: break;
            }
        }

        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            for (int i = 0; i < MAX_BUTTONS; i++) {
                if (PointInRect(e.button.x, e.button.y, &menu->buttons[i].rect)) {
                    menu->action  = i + 1;   /* 1‑indexed */
                    menu->running = false;
                }
            }
        }
    }
}

/* ── Render ─────────────────────────────────────────────────── */

void MainMenu_Render(MainMenu *menu)
{
    SDL_RenderClear(menu->renderer);

    /* Background */
    if (menu->background)
        SDL_RenderCopy(menu->renderer, menu->background, NULL, NULL);

    /* Logo – right side */
    if (menu->logo) {
        SDL_Rect lr = {SCREEN_WIDTH - 600, 100, 400, 300};
        SDL_RenderCopy(menu->renderer, menu->logo, NULL, &lr);
    }

    /* Game title text */
    if (menu->titleFont) {
        SDL_Color white = {255, 255, 255, 255};
        SDL_Texture *title = RenderText(menu->renderer, menu->titleFont,
                                        "Nom du Jeu", white);
        if (title) {
            SDL_Rect tr = {SCREEN_WIDTH - 550, 420, 300, 80};
            SDL_RenderCopy(menu->renderer, title, NULL, &tr);
            SDL_DestroyTexture(title);
        }
    }

    /* Buttons */
    for (int i = 0; i < MAX_BUTTONS; i++) {
        Button *b = &menu->buttons[i];
        SDL_Texture *tex = b->hovered ? b->tex_hover : b->tex_normal;
        if (tex)
            SDL_RenderCopy(menu->renderer, tex, NULL, &b->rect);
        else {
            /* Fallback: draw rectangle */
            SDL_SetRenderDrawColor(menu->renderer,
                                   b->hovered ? 200 : 100,
                                   b->hovered ? 200 : 100,
                                   b->hovered ? 200 : 100, 255);
            SDL_RenderFillRect(menu->renderer, &b->rect);
            SDL_SetRenderDrawColor(menu->renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(menu->renderer, &b->rect);
        }
    }



    SDL_RenderPresent(menu->renderer);
}

/* ── Cleanup ────────────────────────────────────────────────── */

void MainMenu_Cleanup(MainMenu *menu)
{
    for (int i = 0; i < MAX_BUTTONS; i++) {
        if (menu->buttons[i].tex_normal) SDL_DestroyTexture(menu->buttons[i].tex_normal);
        if (menu->buttons[i].tex_hover)  SDL_DestroyTexture(menu->buttons[i].tex_hover);
    }
    if (menu->background)  SDL_DestroyTexture(menu->background);
    if (menu->logo)        SDL_DestroyTexture(menu->logo);
    if (menu->bgMusic)     Mix_FreeMusic(menu->bgMusic);
    if (menu->hoverSound)  Mix_FreeChunk(menu->hoverSound);
    if (menu->font)        TTF_CloseFont(menu->font);
    if (menu->titleFont)   TTF_CloseFont(menu->titleFont);
    if (menu->renderer)    SDL_DestroyRenderer(menu->renderer);
    if (menu->window)      SDL_DestroyWindow(menu->window);
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

/* ── Main loop wrapper ──────────────────────────────────────── */

int MainMenu_Run(MainMenu *menu)
{
    while (menu->running) {
        MainMenu_HandleEvents(menu);
        MainMenu_Render(menu);
        SDL_Delay(16);
    }
    return menu->action;
}
