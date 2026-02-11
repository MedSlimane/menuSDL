/*  scoremenu.c – Best Scores sub-menu (SDL2)
 *  View 1 : Name / pseudo input + Valider
 *  View 2 : Top 3 scores with stars + Quitter / Retour
 */

#include "scoremenu.h"

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

int ScoreMenu_Init(ScoreMenu* menu) {
    memset(menu, 0, sizeof(*menu));

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return 0;
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    TTF_Init();
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) return 0;

    menu->window = SDL_CreateWindow(
        "Sous Menu Meilleurs Scores", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!menu->window) return 0;

    menu->renderer = SDL_CreateRenderer(
        menu->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!menu->renderer) return 0;

    menu->font = TTF_OpenFont("assets/font.ttf", 32);
    menu->titleFont = TTF_OpenFont("assets/font.ttf", 56);

    menu->background =
        IMG_LoadTexture(menu->renderer, "assets/background3.png");

    int cx = SCREEN_WIDTH / 2;

    /* View 1 – input */
    menu->btnValider =
        MakeBtn(menu->renderer, cx - 100, 600, 200, 80,
                "assets/btn_valider.png", "assets/btn_valider_h.png");

    /* View 2 – scores */
    menu->btnQuitter =
        MakeBtn(menu->renderer, cx - 300, SCREEN_HEIGHT - 150, 200, 80,
                "assets/btn_quitter.png", "assets/btn_quitter_h.png");
    menu->btnRetour =
        MakeBtn(menu->renderer, cx + 100, SCREEN_HEIGHT - 150, 200, 80,
                "assets/btn_retour.png", "assets/btn_retour_h.png");

    /* Star images */
    menu->starTextures[0] = IMG_LoadTexture(menu->renderer, "assets/star1.png");
    menu->starTextures[1] = IMG_LoadTexture(menu->renderer, "assets/star2.png");
    menu->starTextures[2] = IMG_LoadTexture(menu->renderer, "assets/star3.png");

    /* Dummy top scores */
    strcpy(menu->topScores[0].name, "Player1");
    menu->topScores[0].score = 5000;
    strcpy(menu->topScores[1].name, "Player2");
    menu->topScores[1].score = 3000;
    strcpy(menu->topScores[2].name, "Player3");
    menu->topScores[2].score = 1000;

    menu->bgMusic = Mix_LoadMUS("assets/victory.mp3");
    menu->hoverSound = Mix_LoadWAV("assets/hover.wav");

    SDL_StartTextInput();

    menu->inputLen = 0;
    menu->inputText[0] = '\0';
    menu->view = SCORE_VIEW_INPUT;
    menu->running = 1;
    menu->action = 0;
    return 1;
}

/* ── Events ──────────────────────────────────────────────── */

void ScoreMenu_HandleEvents(ScoreMenu* menu) {
    int mx, my;
    SDL_GetMouseState(&mx, &my);

    if (menu->view == SCORE_VIEW_INPUT) {
        UpdateHover(&menu->btnValider, mx, my, menu->hoverSound);
    } else {
        UpdateHover(&menu->btnQuitter, mx, my, menu->hoverSound);
        UpdateHover(&menu->btnRetour, mx, my, menu->hoverSound);
    }

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            menu->running = 0;
            return;
        }

        /* Text input for name */
        if (menu->view == SCORE_VIEW_INPUT && e.type == SDL_TEXTINPUT) {
            if (menu->inputLen + (int)strlen(e.text.text) < MAX_NAME_LEN) {
                strcat(menu->inputText, e.text.text);
                menu->inputLen = (int)strlen(menu->inputText);
            }
        }

        if (e.type == SDL_KEYDOWN) {
            if (menu->view == SCORE_VIEW_INPUT) {
                if (e.key.keysym.sym == SDLK_BACKSPACE && menu->inputLen > 0) {
                    menu->inputText[--menu->inputLen] = '\0';
                }
                if (e.key.keysym.sym == SDLK_RETURN) {
                    menu->view = SCORE_VIEW_DISPLAY;
                    if (menu->bgMusic) Mix_PlayMusic(menu->bgMusic, -1);
                }
            } else {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    menu->action = 2;
                    menu->running = 0;
                }
                if (e.key.keysym.sym == SDLK_e) {
                    menu->action = 3;
                    menu->running = 0;
                }
            }
        }

        if (e.type == SDL_MOUSEBUTTONDOWN &&
            e.button.button == SDL_BUTTON_LEFT) {
            int cx = e.button.x, cy = e.button.y;
            if (menu->view == SCORE_VIEW_INPUT) {
                if (PointInRect(cx, cy, &menu->btnValider.rect)) {
                    menu->view = SCORE_VIEW_DISPLAY;
                    if (menu->bgMusic) Mix_PlayMusic(menu->bgMusic, -1);
                }
            } else {
                if (PointInRect(cx, cy, &menu->btnQuitter.rect)) {
                    menu->action = 2;
                    menu->running = 0;
                }
                if (PointInRect(cx, cy, &menu->btnRetour.rect)) {
                    menu->action = 1;
                    menu->running = 0;
                }
            }
        }
    }
}

/* ── Render ──────────────────────────────────────────────── */

void ScoreMenu_Render(ScoreMenu* menu) {
    SDL_RenderClear(menu->renderer);
    if (menu->background)
        SDL_RenderCopy(menu->renderer, menu->background, NULL, NULL);

    if (menu->view == SCORE_VIEW_INPUT) {
        /* Title label */
        if (menu->titleFont) {
            SDL_Color w = {255, 255, 255, 255};
            SDL_Surface* surf = TTF_RenderUTF8_Blended(
                menu->titleFont, "Saisir Nom / Pseudo nom :", w);
            if (surf) {
                SDL_Texture* t =
                    SDL_CreateTextureFromSurface(menu->renderer, surf);
                int tw = surf->w, th = surf->h;
                SDL_Rect r = {(SCREEN_WIDTH - tw) / 2, 280, tw, th};
                SDL_RenderCopy(menu->renderer, t, NULL, &r);
                SDL_DestroyTexture(t);
                SDL_FreeSurface(surf);
            }
        }

        /* Input box */
        int bx = SCREEN_WIDTH / 2 - 250, by = 420;
        SDL_Rect inputRect = {bx, by, 500, 70};
        SDL_SetRenderDrawColor(menu->renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(menu->renderer, &inputRect);
        SDL_SetRenderDrawColor(menu->renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(menu->renderer, &inputRect);

        /* Input text */
        if (menu->font && menu->inputLen > 0) {
            SDL_Color blk = {0, 0, 0, 255};
            SDL_Surface* surf =
                TTF_RenderUTF8_Blended(menu->font, menu->inputText, blk);
            if (surf) {
                SDL_Texture* it =
                    SDL_CreateTextureFromSurface(menu->renderer, surf);
                int tw = surf->w, th = surf->h;
                SDL_Rect r = {bx + 10, by + (70 - th) / 2, tw, th};
                SDL_RenderCopy(menu->renderer, it, NULL, &r);
                SDL_DestroyTexture(it);
                SDL_FreeSurface(surf);
            }
        }

        /* Cursor blink */
        if ((SDL_GetTicks() / 500) % 2 == 0) {
            int curX = bx + 10;
            if (menu->font && menu->inputLen > 0) {
                int tw2;
                TTF_SizeUTF8(menu->font, menu->inputText, &tw2, NULL);
                curX += tw2;
            }
            SDL_SetRenderDrawColor(menu->renderer, 0, 0, 0, 255);
            SDL_RenderDrawLine(menu->renderer, curX, by + 10, curX, by + 60);
        }

        DrawButton(menu->renderer, &menu->btnValider);

    } else {
        /* Scores view */
        if (menu->titleFont) {
            SDL_Color w = {255, 255, 255, 255};
            SDL_Surface* surf =
                TTF_RenderUTF8_Blended(menu->titleFont, "Meilleurs Scores", w);
            if (surf) {
                SDL_Texture* t =
                    SDL_CreateTextureFromSurface(menu->renderer, surf);
                int tw = surf->w, th = surf->h;
                SDL_Rect r = {(SCREEN_WIDTH - tw) / 2, 100, tw, th};
                SDL_RenderCopy(menu->renderer, t, NULL, &r);
                SDL_DestroyTexture(t);
                SDL_FreeSurface(surf);
            }
        }

        /* Stars + scores */
        int sy = 250;
        for (int i = 0; i < TOP_SCORES; i++) {
            if (menu->starTextures[i]) {
                SDL_Rect sr = {SCREEN_WIDTH / 2 - 300, sy, 128, 128};
                SDL_RenderCopy(menu->renderer, menu->starTextures[i], NULL,
                               &sr);
            }

            if (menu->font) {
                char buf[64];
                snprintf(buf, sizeof(buf), "%s  -  %d pts",
                         menu->topScores[i].name, menu->topScores[i].score);
                SDL_Color w = {255, 255, 255, 255};
                SDL_Surface* surf = TTF_RenderUTF8_Blended(menu->font, buf, w);
                if (surf) {
                    SDL_Texture* st =
                        SDL_CreateTextureFromSurface(menu->renderer, surf);
                    int tw = surf->w, th = surf->h;
                    SDL_Rect r = {SCREEN_WIDTH / 2 - 140, sy + 40, tw, th};
                    SDL_RenderCopy(menu->renderer, st, NULL, &r);
                    SDL_DestroyTexture(st);
                    SDL_FreeSurface(surf);
                }
            }
            sy += 160;
        }

        DrawButton(menu->renderer, &menu->btnQuitter);
        DrawButton(menu->renderer, &menu->btnRetour);
    }

    SDL_RenderPresent(menu->renderer);
}

/* ── Cleanup ─────────────────────────────────────────────── */

void ScoreMenu_Cleanup(ScoreMenu* menu) {
    SDL_StopTextInput();
    Button* all[] = {&menu->btnValider, &menu->btnQuitter, &menu->btnRetour};
    for (int i = 0; i < 3; i++) {
        if (all[i]->tex_normal) SDL_DestroyTexture(all[i]->tex_normal);
        if (all[i]->tex_hover) SDL_DestroyTexture(all[i]->tex_hover);
    }
    for (int i = 0; i < TOP_SCORES; i++)
        if (menu->starTextures[i]) SDL_DestroyTexture(menu->starTextures[i]);
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

int ScoreMenu_Run(ScoreMenu* menu) {
    while (menu->running) {
        ScoreMenu_HandleEvents(menu);
        ScoreMenu_Render(menu);
        SDL_Delay(16);
    }
    return menu->action;
}
