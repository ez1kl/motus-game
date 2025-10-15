#ifndef RENDER_H
#define RENDER_H
#include "common.h"
void drawText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, SDL_Color color);
void drawButton(SDL_Renderer *renderer, TTF_Font *font, SDL_Rect rect, const char *text, SDL_Color textColor, SDL_Color bgColor);
void renderMenu(SDL_Renderer *renderer, TTF_Font *font);
void renderDifficulty(SDL_Renderer *renderer, TTF_Font *font, const char *pseudo, int activeField, Difficulty chosenDiff);
void renderGame(SDL_Renderer *renderer, TTF_Font *font, const MotusGame *game, const Player *player, const char *currentGuess);
void renderGameOver(SDL_Renderer *renderer, TTF_Font *font, const MotusGame *game, const Player *player);
void renderHelpPopup(SDL_Renderer *renderer, TTF_Font *font);
#endif
