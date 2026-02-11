/*  playermenu.c – Player sub-menu (SDL2)
 *  View 1 : Mono joueur / Multi joueurs / Retour
 *  View 2 : Avatar1/2  Input1/2  Valider  Retour
 */

#include "playermenu.h"

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

static void DrawSelection(SDL_Renderer* r, SDL_Rect* rect, int selected) {
    if (!selected) return;
    SDL_SetRenderDrawColor(r, 0, 255, 0, 255);
    SDL_Rect outer = {rect->x - 4, rect->y - 4, rect->w + 8, rect->h + 8};
    SDL_RenderDrawRect(r, &outer);
    outer.x--;
    outer.y--;
    outer.w += 2;
    outer.h += 2;
    SDL_RenderDrawRect(r, &outer);
}

/* ── Init ────────────────────────────────────────────────── */

int PlayerMenu_Init(PlayerMenu* menu) {
    memset(menu, 0, sizeof(*menu));

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return 0;
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    TTF_Init();
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) return 0;

    menu->window = SDL_CreateWindow("Sous Menu Joueur", SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                    SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
    menu->btnMono = MakeBtn(menu->renderer, cx - 450, 400, 400, 100,
                            "assets/btn_mono.png", "assets/btn_mono_h.png");
    menu->btnMulti = MakeBtn(menu->renderer, cx + 50, 400, 400, 100,
                             "assets/btn_multi.png", "assets/btn_multi_h.png");
    menu->btnRetour1 =
        MakeBtn(menu->renderer, SCREEN_WIDTH - 350, SCREEN_HEIGHT - 150, 200,
                80, "assets/btn_retour.png", "assets/btn_retour_h.png");

    /* View 2 */
    menu->btnAvatar1 =
        MakeBtn(menu->renderer, cx - 400, 200, 300, 120,
                "assets/btn_avatar1.png", "assets/btn_avatar1_h.png");
    menu->btnAvatar2 =
        MakeBtn(menu->renderer, cx + 100, 200, 300, 120,
                "assets/btn_avatar2.png", "assets/btn_avatar2_h.png");
    menu->btnInput1 =
        MakeBtn(menu->renderer, cx - 400, 400, 300, 100,
                "assets/btn_input1.png", "assets/btn_input1_h.png");
    menu->btnInput2 =
        MakeBtn(menu->renderer, cx + 100, 400, 300, 100,
                "assets/btn_input2.png", "assets/btn_input2_h.png");
    menu->btnValider =
        MakeBtn(menu->renderer, cx - 150, 580, 300, 100,
                "assets/btn_valider.png", "assets/btn_valider_h.png");
    menu->btnRetour2 =
        MakeBtn(menu->renderer, SCREEN_WIDTH - 350, SCREEN_HEIGHT - 150, 200,
                80, "assets/btn_retour.png", "assets/btn_retour_h.png");

    menu->bgMusic = Mix_LoadMUS("assets/music_option.mp3");
    menu->hoverSound = Mix_LoadWAV("assets/hover.wav");
    if (menu->bgMusic) Mix_PlayMusic(menu->bgMusic, -1);

    menu->selectedAvatar = 0;
    menu->selectedInput = 0;
    menu->playerMode = 0;
    menu->view = PLAYER_VIEW_MODE;
    menu->running = 1;
    menu->action = 0;
    return 1;
}

/* ── Events ──────────────────────────────────────────────── */

void PlayerMenu_HandleEvents(PlayerMenu* menu) {
    int mx, my;
    SDL_GetMouseState(&mx, &my);

    if (menu->view == PLAYER_VIEW_MODE) {
        UpdateHover(&menu->btnMono, mx, my, menu->hoverSound);
        UpdateHover(&menu->btnMulti, mx, my, menu->hoverSound);
        UpdateHover(&menu->btnRetour1, mx, my, menu->hoverSound);
    } else {
        UpdateHover(&menu->btnAvatar1, mx, my, menu->hoverSound);
        UpdateHover(&menu->btnAvatar2, mx, my, menu->hoverSound);
        UpdateHover(&menu->btnInput1, mx, my, menu->hoverSound);
        UpdateHover(&menu->btnInput2, mx, my, menu->hoverSound);
        UpdateHover(&menu->btnValider, mx, my, menu->hoverSound);
        UpdateHover(&menu->btnRetour2, mx, my, menu->hoverSound);
    }

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            menu->running = 0;
            return;
        }

        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_RETURN &&
                menu->view == PLAYER_VIEW_CONFIG) {
                menu->action = 1;
                menu->running = 0;
            }
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                if (menu->view == PLAYER_VIEW_CONFIG)
                    menu->view = PLAYER_VIEW_MODE;
                else {
                    menu->action = 2;
                    menu->running = 0;
                }
            }
        }

        if (e.type == SDL_MOUSEBUTTONDOWN &&
            e.button.button == SDL_BUTTON_LEFT) {
            int cx = e.button.x, cy = e.button.y;
            if (menu->view == PLAYER_VIEW_MODE) {
                if (PointInRect(cx, cy, &menu->btnMono.rect)) {
                    menu->playerMode = 1;
                    menu->view = PLAYER_VIEW_CONFIG;
                }
                if (PointInRect(cx, cy, &menu->btnMulti.rect)) {
                    menu->playerMode = 2;
                    menu->view = PLAYER_VIEW_CONFIG;
                }
                if (PointInRect(cx, cy, &menu->btnRetour1.rect)) {
                    menu->action = 2;
                    menu->running = 0;
                }
            } else {
                if (PointInRect(cx, cy, &menu->btnAvatar1.rect))
                    menu->selectedAvatar = 1;
                if (PointInRect(cx, cy, &menu->btnAvatar2.rect))
                    menu->selectedAvatar = 2;
                if (PointInRect(cx, cy, &menu->btnInput1.rect))
                    menu->selectedInput = 1;
                if (PointInRect(cx, cy, &menu->btnInput2.rect))
                    menu->selectedInput = 2;
                if (PointInRect(cx, cy, &menu->btnValider.rect)) {
                    menu->action = 1;
                    menu->running = 0;
                }
                if (PointInRect(cx, cy, &menu->btnRetour2.rect)) {
                    menu->view = PLAYER_VIEW_MODE;
                }
            }
        }
    }
}

/* ── Render ──────────────────────────────────────────────── */

void PlayerMenu_Render(PlayerMenu* menu) {
    SDL_RenderClear(menu->renderer);
    if (menu->background)
        SDL_RenderCopy(menu->renderer, menu->background, NULL, NULL);

    if (menu->view == PLAYER_VIEW_MODE) {
        DrawButton(menu->renderer, &menu->btnMono);
        DrawButton(menu->renderer, &menu->btnMulti);
        DrawButton(menu->renderer, &menu->btnRetour1);

    } else {
        DrawButton(menu->renderer, &menu->btnAvatar1);
        DrawButton(menu->renderer, &menu->btnAvatar2);
        DrawButton(menu->renderer, &menu->btnInput1);
        DrawButton(menu->renderer, &menu->btnInput2);
        DrawButton(menu->renderer, &menu->btnValider);
        DrawButton(menu->renderer, &menu->btnRetour2);

        /* Selection highlights */
        DrawSelection(menu->renderer, &menu->btnAvatar1.rect,
                      menu->selectedAvatar == 1);
        DrawSelection(menu->renderer, &menu->btnAvatar2.rect,
                      menu->selectedAvatar == 2);
        DrawSelection(menu->renderer, &menu->btnInput1.rect,
                      menu->selectedInput == 1);
        DrawSelection(menu->renderer, &menu->btnInput2.rect,
                      menu->selectedInput == 2);

        /* Mode label */
        if (menu->titleFont) {
            const char* mode =
                menu->playerMode == 1 ? "Mono joueur" : "Multi joueurs";
            SDL_Color w = {255, 255, 255, 255};
            SDL_Surface* surf =
                TTF_RenderUTF8_Blended(menu->titleFont, mode, w);
            if (surf) {
                SDL_Texture* mt =
                    SDL_CreateTextureFromSurface(menu->renderer, surf);
                int tw = surf->w, th = surf->h;
                SDL_Rect r = {(SCREEN_WIDTH - tw) / 2, 80, tw, th};
                SDL_RenderCopy(menu->renderer, mt, NULL, &r);
                SDL_DestroyTexture(mt);
                SDL_FreeSurface(surf);
            }
        }
    }

    SDL_RenderPresent(menu->renderer);
}

/* ── Cleanup ─────────────────────────────────────────────── */

void PlayerMenu_Cleanup(PlayerMenu* menu) {
    Button* all[] = {&menu->btnMono,    &menu->btnMulti,   &menu->btnRetour1,
                     &menu->btnAvatar1, &menu->btnAvatar2, &menu->btnInput1,
                     &menu->btnInput2,  &menu->btnValider, &menu->btnRetour2};
    for (int i = 0; i < 9; i++) {
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

int PlayerMenu_Run(PlayerMenu* menu) {
    while (menu->running) {
        PlayerMenu_HandleEvents(menu);
        PlayerMenu_Render(menu);
        SDL_Delay(16);
    }
    return menu->action;
}
