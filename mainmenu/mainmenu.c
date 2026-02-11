/*  mainmenu.c – Main Menu (SDL2)
 *  Buttons : Jouer, Options, Meilleurs Scores, Histoire, Quitter
 */

#include "mainmenu.h"

#include <stdio.h>
#include <string.h>

/* ── Helpers ──────────────────────────────────────────────── */

static int PointInRect(int x, int y, const SDL_Rect* r) {
    return (x >= r->x && x <= r->x + r->w && y >= r->y && y <= r->y + r->h);
}

static Button MakeButton(SDL_Renderer* renderer, int x, int y, int w, int h,
                         const char* normalImg, const char* hoverImg) {
    Button b;
    memset(&b, 0, sizeof(b));
    b.rect = (SDL_Rect){x, y, w, h};
    b.tex_normal = IMG_LoadTexture(renderer, normalImg);
    b.tex_hover = IMG_LoadTexture(renderer, hoverImg);
    b.hovered = 0;
    b.was_hovered = 0;
    return b;
}

/* ── Init ────────────────────────────────────────────────── */

int MainMenu_Init(MainMenu* menu) {
    memset(menu, 0, sizeof(*menu));

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 0;
    }
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    TTF_Init();
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Mix_OpenAudio: %s\n", Mix_GetError());
        return 0;
    }

    menu->window = SDL_CreateWindow("Menu Principal", SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                    SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!menu->window) return 0;

    menu->renderer = SDL_CreateRenderer(
        menu->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!menu->renderer) return 0;

    /* Polices */
    menu->font = TTF_OpenFont("assets/font.ttf", 32);
    menu->titleFont = TTF_OpenFont("assets/font.ttf", 64);

    /* Arriere-plan et logo */
    menu->background =
        IMG_LoadTexture(menu->renderer, "assets/background1.png");
    menu->logo = IMG_LoadTexture(menu->renderer, "assets/logo.png");

    /* Boutons */
    int bx = 150, by = 180, bw = 400, bh = 100, gap = 40;
    menu->buttons[0] =
        MakeButton(menu->renderer, bx, by + 0 * (bh + gap), bw, bh,
                   "assets/btn_jouer.png", "assets/btn_jouer_h.png");
    menu->buttons[1] =
        MakeButton(menu->renderer, bx, by + 1 * (bh + gap), bw, bh,
                   "assets/btn_options.png", "assets/btn_options_h.png");
    menu->buttons[2] =
        MakeButton(menu->renderer, bx, by + 2 * (bh + gap), bw, bh,
                   "assets/btn_scores.png", "assets/btn_scores_h.png");
    menu->buttons[3] =
        MakeButton(menu->renderer, bx, by + 3 * (bh + gap), bw, bh,
                   "assets/btn_histoire.png", "assets/btn_histoire_h.png");
    menu->buttons[4] =
        MakeButton(menu->renderer, SCREEN_WIDTH - 400, SCREEN_HEIGHT - 150, 200,
                   80, "assets/btn_quitter.png", "assets/btn_quitter_h.png");

    /* Audio */
    menu->bgMusic = Mix_LoadMUS("assets/music.mp3");
    menu->hoverSound = Mix_LoadWAV("assets/hover.wav");
    if (menu->bgMusic) Mix_PlayMusic(menu->bgMusic, -1);

    menu->running = 1;
    menu->action = 0;
    return 1;
}

/* ── Events ──────────────────────────────────────────────── */

void MainMenu_HandleEvents(MainMenu* menu) {
    SDL_Event e;
    int mx, my;
    SDL_GetMouseState(&mx, &my);

    /* Mise a jour hover */
    for (int i = 0; i < MAX_BUTTONS; i++) {
        menu->buttons[i].hovered = PointInRect(mx, my, &menu->buttons[i].rect);
        if (menu->buttons[i].hovered && !menu->buttons[i].was_hovered) {
            if (menu->hoverSound) Mix_PlayChannel(-1, menu->hoverSound, 0);
        }
        menu->buttons[i].was_hovered = menu->buttons[i].hovered;
    }

    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            menu->running = 0;
            menu->action = 5;
        }

        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_j:
                    menu->action = 1;
                    menu->running = 0;
                    break;
                case SDLK_o:
                    menu->action = 2;
                    menu->running = 0;
                    break;
                case SDLK_m:
                    menu->action = 3;
                    menu->running = 0;
                    break;
                case SDLK_ESCAPE:
                    menu->action = 5;
                    menu->running = 0;
                    break;
                default:
                    break;
            }
        }

        if (e.type == SDL_MOUSEBUTTONDOWN &&
            e.button.button == SDL_BUTTON_LEFT) {
            for (int i = 0; i < MAX_BUTTONS; i++) {
                if (PointInRect(e.button.x, e.button.y,
                                &menu->buttons[i].rect)) {
                    menu->action = i + 1;
                    menu->running = 0;
                }
            }
        }
    }
}

/* ── Render ──────────────────────────────────────────────── */

void MainMenu_Render(MainMenu* menu) {
    SDL_RenderClear(menu->renderer);

    /* Arriere-plan */
    if (menu->background)
        SDL_RenderCopy(menu->renderer, menu->background, NULL, NULL);

    /* Logo */
    if (menu->logo) {
        SDL_Rect lr = {SCREEN_WIDTH - 600, 100, 400, 300};
        SDL_RenderCopy(menu->renderer, menu->logo, NULL, &lr);
    }

    /* Titre du jeu */
    if (menu->titleFont) {
        SDL_Color white = {255, 255, 255, 255};
        SDL_Surface* surf =
            TTF_RenderUTF8_Blended(menu->titleFont, "Nom du Jeu", white);
        if (surf) {
            SDL_Texture* title =
                SDL_CreateTextureFromSurface(menu->renderer, surf);
            SDL_Rect tr = {SCREEN_WIDTH - 550, 420, 300, 80};
            SDL_RenderCopy(menu->renderer, title, NULL, &tr);
            SDL_DestroyTexture(title);
            SDL_FreeSurface(surf);
        }
    }

    /* Boutons */
    for (int i = 0; i < MAX_BUTTONS; i++) {
        Button* b = &menu->buttons[i];
        SDL_Texture* tex = b->hovered ? b->tex_hover : b->tex_normal;
        if (tex)
            SDL_RenderCopy(menu->renderer, tex, NULL, &b->rect);
        else {
            SDL_SetRenderDrawColor(menu->renderer, b->hovered ? 200 : 100,
                                   b->hovered ? 200 : 100,
                                   b->hovered ? 200 : 100, 255);
            SDL_RenderFillRect(menu->renderer, &b->rect);
            SDL_SetRenderDrawColor(menu->renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(menu->renderer, &b->rect);
        }
    }

    SDL_RenderPresent(menu->renderer);
}

/* ── Cleanup ─────────────────────────────────────────────── */

void MainMenu_Cleanup(MainMenu* menu) {
    for (int i = 0; i < MAX_BUTTONS; i++) {
        if (menu->buttons[i].tex_normal)
            SDL_DestroyTexture(menu->buttons[i].tex_normal);
        if (menu->buttons[i].tex_hover)
            SDL_DestroyTexture(menu->buttons[i].tex_hover);
    }
    if (menu->background) SDL_DestroyTexture(menu->background);
    if (menu->logo) SDL_DestroyTexture(menu->logo);
    if (menu->bgMusic) Mix_FreeMusic(menu->bgMusic);
    if (menu->hoverSound) Mix_FreeChunk(menu->hoverSound);
    if (menu->font) TTF_CloseFont(menu->font);
    if (menu->titleFont) TTF_CloseFont(menu->titleFont);
    if (menu->renderer) SDL_DestroyRenderer(menu->renderer);
    if (menu->window) SDL_DestroyWindow(menu->window);
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

/* ── Boucle principale ──────────────────────────────────── */

int MainMenu_Run(MainMenu* menu) {
    while (menu->running) {
        MainMenu_HandleEvents(menu);
        MainMenu_Render(menu);
        SDL_Delay(16);
    }
    return menu->action;
}
