/*  savemenu.c – Save / Load sub-menu (SDL2)
 *  View 1 : Voulez-vous sauvegarder votre jeu? -> OUI / NON
 *  View 2 : Charger le jeu / Nouvelle Partie
 */

#include "savemenu.h"

#include <stdio.h>
#include <string.h>

/* ── Helpers ──────────────────────────────────────────────── */

static int PointInRect(int x, int y, const SDL_Rect* r) {
    return (x >= r->x && x < r->x + r->w && y >= r->y && y < r->y + r->h);
}

static Button MakeBtn(SDL_Renderer* r, int x, int y, int w, int h,
                      const char* n, const char* hv) {
    Button b;
    memset(&b, 0, sizeof(b));
    b.rect = (SDL_Rect){x, y, w, h};
    b.tex_normal = IMG_LoadTexture(r, n);
    b.tex_hover = IMG_LoadTexture(r, hv);
    return b;
}

static void UpdateHover(Button* b, int mx, int my, Mix_Chunk* snd) {
    b->hovered = PointInRect(mx, my, &b->rect);
    if (b->hovered && !b->was_hovered && snd) Mix_PlayChannel(-1, snd, 0);
    b->was_hovered = b->hovered;
}

static void DrawButton(SDL_Renderer* r, Button* b) {
    SDL_Texture* tex = b->hovered ? b->tex_hover : b->tex_normal;
    if (tex)
        SDL_RenderCopy(r, tex, NULL, &b->rect);
    else {
        SDL_SetRenderDrawColor(r, b->hovered ? 200 : 100,
                               b->hovered ? 200 : 100, b->hovered ? 200 : 100,
                               255);
        SDL_RenderFillRect(r, &b->rect);
        SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
        SDL_RenderDrawRect(r, &b->rect);
    }
}

/* ── Init ────────────────────────────────────────────────── */

int SaveMenu_Init(SaveMenu* menu) {
    memset(menu, 0, sizeof(*menu));

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return 0;
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    TTF_Init();
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) return 0;

    menu->window = SDL_CreateWindow(
        "Sous Menu Sauvegarde", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!menu->window) return 0;

    menu->renderer = SDL_CreateRenderer(
        menu->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!menu->renderer) return 0;

    menu->font = TTF_OpenFont("assets/font.ttf", 32);
    menu->titleFont = TTF_OpenFont("assets/font.ttf", 48);

    menu->background =
        IMG_LoadTexture(menu->renderer, "assets/background2.png");

    int cx = SCREEN_WIDTH / 2;

    /* View 1 */
    menu->btnOui = MakeBtn(menu->renderer, cx - 300, 550, 200, 80,
                           "assets/btn_oui.png", "assets/btn_oui_h.png");
    menu->btnNon = MakeBtn(menu->renderer, cx + 100, 550, 200, 80,
                           "assets/btn_non.png", "assets/btn_non_h.png");

    /* View 2 */
    menu->btnCharger =
        MakeBtn(menu->renderer, cx - 200, 380, 400, 100,
                "assets/btn_charger.png", "assets/btn_charger_h.png");
    menu->btnNouvelle =
        MakeBtn(menu->renderer, cx - 200, 530, 400, 100,
                "assets/btn_nouvelle.png", "assets/btn_nouvelle_h.png");

    menu->bgMusic = Mix_LoadMUS("assets/music_option.mp3");
    menu->hoverSound = Mix_LoadWAV("assets/hover.wav");
    if (menu->bgMusic) Mix_PlayMusic(menu->bgMusic, -1);

    menu->view = SAVE_VIEW_ASK;
    menu->running = 1;
    menu->action = 0;
    return 1;
}

/* ── Events ──────────────────────────────────────────────── */

void SaveMenu_HandleEvents(SaveMenu* menu) {
    int mx, my;
    SDL_GetMouseState(&mx, &my);

    if (menu->view == SAVE_VIEW_ASK) {
        UpdateHover(&menu->btnOui, mx, my, menu->hoverSound);
        UpdateHover(&menu->btnNon, mx, my, menu->hoverSound);
    } else {
        UpdateHover(&menu->btnCharger, mx, my, menu->hoverSound);
        UpdateHover(&menu->btnNouvelle, mx, my, menu->hoverSound);
    }

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            menu->running = 0;
            return;
        }

        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_n && menu->view == SAVE_VIEW_LOAD) {
                menu->action = 2;
                menu->running = 0;
            }
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                menu->action = 3;
                menu->running = 0;
            }
        }

        if (e.type == SDL_MOUSEBUTTONDOWN &&
            e.button.button == SDL_BUTTON_LEFT) {
            int cx = e.button.x, cy = e.button.y;
            if (menu->view == SAVE_VIEW_ASK) {
                if (PointInRect(cx, cy, &menu->btnOui.rect))
                    menu->view = SAVE_VIEW_LOAD;
                if (PointInRect(cx, cy, &menu->btnNon.rect)) {
                    menu->action = 3;
                    menu->running = 0;
                }
            } else {
                if (PointInRect(cx, cy, &menu->btnCharger.rect)) {
                    menu->action = 1;
                    menu->running = 0;
                }
                if (PointInRect(cx, cy, &menu->btnNouvelle.rect)) {
                    menu->action = 2;
                    menu->running = 0;
                }
            }
        }
    }
}

/* ── Render ──────────────────────────────────────────────── */

void SaveMenu_Render(SaveMenu* menu) {
    SDL_RenderClear(menu->renderer);
    if (menu->background)
        SDL_RenderCopy(menu->renderer, menu->background, NULL, NULL);

    if (menu->view == SAVE_VIEW_ASK) {
        /* Question */
        if (menu->titleFont) {
            SDL_Color w = {255, 255, 255, 255};
            SDL_Surface* surf = TTF_RenderUTF8_Blended(
                menu->titleFont, "Voulez-vous sauvegarder votre jeu?", w);
            if (surf) {
                SDL_Texture* q =
                    SDL_CreateTextureFromSurface(menu->renderer, surf);
                int tw = surf->w, th = surf->h;
                SDL_Rect r = {(SCREEN_WIDTH - tw) / 2, 350, tw, th};
                SDL_RenderCopy(menu->renderer, q, NULL, &r);
                SDL_DestroyTexture(q);
                SDL_FreeSurface(surf);
            }
        }
        DrawButton(menu->renderer, &menu->btnOui);
        DrawButton(menu->renderer, &menu->btnNon);

    } else {
        DrawButton(menu->renderer, &menu->btnCharger);
        DrawButton(menu->renderer, &menu->btnNouvelle);
    }

    SDL_RenderPresent(menu->renderer);
}

/* ── Cleanup ─────────────────────────────────────────────── */

void SaveMenu_Cleanup(SaveMenu* menu) {
    Button* all[] = {&menu->btnOui, &menu->btnNon, &menu->btnCharger,
                     &menu->btnNouvelle};
    for (int i = 0; i < 4; i++) {
        if (all[i]->tex_normal) SDL_DestroyTexture(all[i]->tex_normal);
        if (all[i]->tex_hover) SDL_DestroyTexture(all[i]->tex_hover);
    }
    if (menu->background) SDL_DestroyTexture(menu->background);
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

int SaveMenu_Run(SaveMenu* menu) {
    while (menu->running) {
        SaveMenu_HandleEvents(menu);
        SaveMenu_Render(menu);
        SDL_Delay(16);
    }
    return menu->action;
}
