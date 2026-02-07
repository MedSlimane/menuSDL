/*  engimemenu.c – Enigme sub‑menu implementation (SDL2)
 *  View 1 : Quiz / Puzzle buttons  (arrière-plan 4, no music)
 *  View 2 : Quiz title + question band + A / B / C answers
 *           (suspense music starts)
 *  Escape → quit
 */

#include "engimemenu.h"
#include <stdio.h>
#include <string.h>

/* ── Helpers ────────────────────────────────────────────────── */

SDL_Texture *EM_LoadTexture(SDL_Renderer *r, const char *path)
{
    SDL_Surface *s = IMG_Load(path);
    if (!s) { fprintf(stderr, "IMG_Load(%s): %s\n", path, IMG_GetError()); return NULL; }
    SDL_Texture *t = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);
    return t;
}

SDL_Texture *EM_RenderText(SDL_Renderer *r, TTF_Font *f,
                           const char *text, SDL_Color c)
{
    SDL_Surface *s = TTF_RenderUTF8_Blended(f, text, c);
    if (!s) return NULL;
    SDL_Texture *t = SDL_CreateTextureFromSurface(r, s);
    SDL_FreeSurface(s);
    return t;
}

bool EM_PointInRect(int x, int y, const SDL_Rect *r)
{
    return x >= r->x && x < r->x + r->w && y >= r->y && y < r->y + r->h;
}

static Button MakeBtn(SDL_Renderer *r, int x, int y, int w, int h,
                      const char *n, const char *hv)
{
    Button b; memset(&b, 0, sizeof(b));
    b.rect = (SDL_Rect){x, y, w, h};
    b.tex_normal = EM_LoadTexture(r, n);
    b.tex_hover  = EM_LoadTexture(r, hv);
    return b;
}

static void UpdateHover(Button *b, int mx, int my, Mix_Chunk *snd)
{
    b->hovered = EM_PointInRect(mx, my, &b->rect);
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
        SDL_SetRenderDrawColor(r, 255,255,255,255);
        SDL_RenderDrawRect(r, &b->rect);
    }
}

static void DrawLabel(SDL_Renderer *r, TTF_Font *f, const char *txt, SDL_Rect *btn)
{
    if (!f) return;
    SDL_Color blk = {0,0,0,255};
    SDL_Texture *t = EM_RenderText(r, f, txt, blk);
    if (!t) return;
    int tw, th; SDL_QueryTexture(t, NULL, NULL, &tw, &th);
    SDL_Rect dst = {btn->x+(btn->w-tw)/2, btn->y+(btn->h-th)/2, tw, th};
    SDL_RenderCopy(r, t, NULL, &dst);
    SDL_DestroyTexture(t);
}

/* Draw a trapezoid‑like question band (SDL has no polygon fill,
   so we approximate with horizontal lines) */
static void DrawQuestionBand(SDL_Renderer *r, int cx, int y, int w, int h)
{
    int inset = w / 8;   /* slant amount */
    SDL_SetRenderDrawColor(r, 220, 220, 220, 255);
    for (int row = 0; row < h; row++) {
        float t = (float)row / (float)(h - 1);          /* 0 → 1 */
        int left  = cx - w/2 + (int)(inset * (1.0f - t));
        int right = cx + w/2 - (int)(inset * (1.0f - t));
        SDL_RenderDrawLine(r, left, y + row, right, y + row);
    }
    /* Outline */
    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    int topL = cx - w/2 + inset, topR = cx + w/2 - inset;
    int botL = cx - w/2,         botR = cx + w/2;
    SDL_RenderDrawLine(r, topL, y,     topR, y);
    SDL_RenderDrawLine(r, botL, y+h-1, botR, y+h-1);
    SDL_RenderDrawLine(r, topL, y,     botL, y+h-1);
    SDL_RenderDrawLine(r, topR, y,     botR, y+h-1);
}

/* ── Init ───────────────────────────────────────────────────── */

bool EnigmeMenu_Init(EnigmeMenu *menu)
{
    memset(menu, 0, sizeof(*menu));

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return false;
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))       return false;
    if (TTF_Init() < 0)                                 return false;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) return false;

    menu->window = SDL_CreateWindow("Sous Menu Enigme",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!menu->window) return false;

    menu->renderer = SDL_CreateRenderer(menu->window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!menu->renderer) return false;

    menu->font      = TTF_OpenFont("assets/font.ttf", 32);
    menu->titleFont = TTF_OpenFont("assets/font.ttf", 72);

    menu->background = EM_LoadTexture(menu->renderer, "assets/background4.png");

    int cx = SCREEN_WIDTH / 2;

    /* View 1 – choice */
    menu->btnQuiz   = MakeBtn(menu->renderer, cx - 350, 400, 300, 120,
                              "assets/btn_quiz.png",   "assets/btn_quiz_h.png");
    menu->btnPuzzle = MakeBtn(menu->renderer, cx + 50,  400, 300, 120,
                              "assets/btn_puzzle.png", "assets/btn_puzzle_h.png");

    /* View 2 – quiz answers */
    int ay = 700, aw = 150, ah = 80, agap = 200;
    int ax = cx - (3 * aw + 2 * agap) / 2;   /* left-align 3 buttons */
    menu->btnA = MakeBtn(menu->renderer, ax,              ay, aw, ah,
                         "assets/btn_a.png", "assets/btn_a_h.png");
    menu->btnB = MakeBtn(menu->renderer, ax + aw + agap,  ay, aw, ah,
                         "assets/btn_b.png", "assets/btn_b_h.png");
    menu->btnC = MakeBtn(menu->renderer, ax + 2*(aw+agap),ay, aw, ah,
                         "assets/btn_c.png", "assets/btn_c_h.png");

    /* Sample quiz question */
    menu->question    = "Quel langage est utilise pour ce projet?";
    menu->answers[0]  = "Python";
    menu->answers[1]  = "C";
    menu->answers[2]  = "Java";
    menu->correctIndex = 1;
    menu->selectedAnswer = -1;

    menu->quizMusic  = Mix_LoadMUS("assets/suspense.mp3");
    menu->hoverSound = Mix_LoadWAV("assets/hover.wav");

    /* No music at start (spec says no music on enigme choice view) */

    menu->view    = ENIGME_VIEW_CHOICE;
    menu->running = true;
    menu->action  = 0;
    return true;
}

/* ── Events ─────────────────────────────────────────────────── */

void EnigmeMenu_HandleEvents(EnigmeMenu *menu)
{
    int mx, my; SDL_GetMouseState(&mx, &my);

    if (menu->view == ENIGME_VIEW_CHOICE) {
        UpdateHover(&menu->btnQuiz,   mx, my, menu->hoverSound);
        UpdateHover(&menu->btnPuzzle, mx, my, menu->hoverSound);
    } else {
        UpdateHover(&menu->btnA, mx, my, menu->hoverSound);
        UpdateHover(&menu->btnB, mx, my, menu->hoverSound);
        UpdateHover(&menu->btnC, mx, my, menu->hoverSound);
    }

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) { menu->running = false; return; }

        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                menu->action = 2; menu->running = false;
            }
        }

        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            int cx = e.button.x, cy = e.button.y;
            if (menu->view == ENIGME_VIEW_CHOICE) {
                if (EM_PointInRect(cx, cy, &menu->btnQuiz.rect)) {
                    menu->view = ENIGME_VIEW_QUIZ;
                    if (menu->quizMusic) Mix_PlayMusic(menu->quizMusic, -1);
                }
                if (EM_PointInRect(cx, cy, &menu->btnPuzzle.rect)) {
                    /* Puzzle mode – placeholder: just print message */
                    printf("Puzzle mode selected (not implemented yet).\n");
                }
            } else {
                if (EM_PointInRect(cx, cy, &menu->btnA.rect)) {
                    menu->selectedAnswer = 0;
                    menu->action = 1; menu->running = false;
                }
                if (EM_PointInRect(cx, cy, &menu->btnB.rect)) {
                    menu->selectedAnswer = 1;
                    menu->action = 1; menu->running = false;
                }
                if (EM_PointInRect(cx, cy, &menu->btnC.rect)) {
                    menu->selectedAnswer = 2;
                    menu->action = 1; menu->running = false;
                }
            }
        }
    }
}

/* ── Render ─────────────────────────────────────────────────── */

void EnigmeMenu_Render(EnigmeMenu *menu)
{
    SDL_RenderClear(menu->renderer);
    if (menu->background) SDL_RenderCopy(menu->renderer, menu->background, NULL, NULL);

    int cx = SCREEN_WIDTH / 2;

    if (menu->view == ENIGME_VIEW_CHOICE) {
        DrawButton(menu->renderer, &menu->btnQuiz);
        DrawButton(menu->renderer, &menu->btnPuzzle);


    } else {
        /* QUIZ title */
        if (menu->titleFont) {
            SDL_Color w = {255, 255, 255, 255};
            SDL_Texture *t = EM_RenderText(menu->renderer, menu->titleFont, "QUIZ", w);
            if (t) {
                int tw, th; SDL_QueryTexture(t, NULL, NULL, &tw, &th);
                SDL_Rect r = {cx - tw/2, 80, tw, th};
                SDL_RenderCopy(menu->renderer, t, NULL, &r);
                SDL_DestroyTexture(t);
            }
        }

        /* Question band (trapezoid shape) */
        DrawQuestionBand(menu->renderer, cx, 280, 900, 100);

        /* Question text inside band */
        if (menu->font && menu->question) {
            SDL_Color blk = {0,0,0,255};
            SDL_Texture *qt = EM_RenderText(menu->renderer, menu->font,
                                            menu->question, blk);
            if (qt) {
                int tw, th; SDL_QueryTexture(qt, NULL, NULL, &tw, &th);
                SDL_Rect r = {cx - tw/2, 280 + (100-th)/2, tw, th};
                SDL_RenderCopy(menu->renderer, qt, NULL, &r);
                SDL_DestroyTexture(qt);
            }
        }

        /* Answer buttons */
        DrawButton(menu->renderer, &menu->btnA);
        DrawButton(menu->renderer, &menu->btnB);
        DrawButton(menu->renderer, &menu->btnC);


    }

    SDL_RenderPresent(menu->renderer);
}

/* ── Cleanup ────────────────────────────────────────────────── */

void EnigmeMenu_Cleanup(EnigmeMenu *menu)
{
    Button *all[] = {&menu->btnQuiz, &menu->btnPuzzle,
                     &menu->btnA, &menu->btnB, &menu->btnC};
    for (int i = 0; i < 5; i++) {
        if (all[i]->tex_normal) SDL_DestroyTexture(all[i]->tex_normal);
        if (all[i]->tex_hover)  SDL_DestroyTexture(all[i]->tex_hover);
    }
    if (menu->background)  SDL_DestroyTexture(menu->background);
    if (menu->quizMusic)   Mix_FreeMusic(menu->quizMusic);
    if (menu->hoverSound)  Mix_FreeChunk(menu->hoverSound);
    if (menu->font)        TTF_CloseFont(menu->font);
    if (menu->titleFont)   TTF_CloseFont(menu->titleFont);
    if (menu->renderer)    SDL_DestroyRenderer(menu->renderer);
    if (menu->window)      SDL_DestroyWindow(menu->window);
    Mix_CloseAudio(); TTF_Quit(); IMG_Quit(); SDL_Quit();
}

/* ── Run ────────────────────────────────────────────────────── */

int EnigmeMenu_Run(EnigmeMenu *menu)
{
    while (menu->running) {
        EnigmeMenu_HandleEvents(menu);
        EnigmeMenu_Render(menu);
        SDL_Delay(16);
    }
    return menu->action;
}
