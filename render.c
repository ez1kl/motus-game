#include "render.h"
#include "common.h"
#include <stdio.h>
#include <string.h>

static void computeLetterColors(const char *secret, const char *guess, SDL_Color *colors) {
    SDL_Color GREEN = {0,255,0,255}, YELLOW = {255,255,0,255}, GREY = {190,190,190,255};
    int len = (int)strlen(secret);
    for (int i = 0; i < len; i++) { colors[i] = GREY; }
    int freqSecret[26] = {0}, freqGuess[26] = {0};
    for (int i = 0; i < len; i++) {
        if (secret[i] == guess[i]) { colors[i] = GREEN; }
        else { freqSecret[secret[i]-'A']++; freqGuess[guess[i]-'A']++; }
    }
    for (int c = 0; c < 26; c++) {
        int available = freqSecret[c] < freqGuess[c] ? freqSecret[c] : freqGuess[c];
        if (available > 0) {
            for (int i = 0; i < len; i++) {
                if ((guess[i]-'A') == c &&
                    colors[i].r==GREY.r && colors[i].g==GREY.g && colors[i].b==GREY.b) {
                    colors[i] = YELLOW;
                    available--;
                    if (available == 0) break;
                }
            }
        }
    }
}

void drawText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
    if (!surface) { printf("Erreur TTF_RenderText: %s\n", TTF_GetError()); return; }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture) {
        SDL_Rect dst = { x, y, surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, NULL, &dst);
        SDL_DestroyTexture(texture);
    }
    SDL_FreeSurface(surface);
}

void drawButton(SDL_Renderer *renderer, TTF_Font *font, SDL_Rect rect, const char *text, SDL_Color textColor, SDL_Color bgColor) {
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, textColor);
    if (surface) {
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture) {
            SDL_Rect dst;
            dst.w = surface->w; dst.h = surface->h;
            dst.x = rect.x + (rect.w - dst.w) / 2;
            dst.y = rect.y + (rect.h - dst.h) / 2;
            SDL_RenderCopy(renderer, texture, NULL, &dst);
            SDL_DestroyTexture(texture);
        }
        SDL_FreeSurface(surface);
    }
}

void renderHelpPopup(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0,0,0,150);
    SDL_Rect overlay = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_RenderFillRect(renderer, &overlay);
    int popupW = 700, popupH = 450;
    int popupX = (WINDOW_WIDTH - popupW) / 2;
    int popupY = (WINDOW_HEIGHT - popupH) / 2;
    SDL_Rect popup = {popupX, popupY, popupW, popupH};
    SDL_SetRenderDrawColor(renderer, 240,240,240,255);
    SDL_RenderFillRect(renderer, &popup);
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderDrawRect(renderer, &popup);
    SDL_Color textColor = {0,0,0,255};
    int lineY = popupY + 40;
    int marginX = popupX + 30;
    drawText(renderer, font, "Regles de Motus :", marginX, lineY, textColor);
    lineY += 40;
    drawText(renderer, font, "- Devinez le mot secret", marginX, lineY, textColor);
    lineY += 30;
    drawText(renderer, font, "  en saisissant vos propositions.", marginX, lineY, textColor);
    lineY += 30;
    drawText(renderer, font, "- Lettre verte indique", marginX, lineY, textColor);
    lineY += 30;
    drawText(renderer, font, "  que la lettre est bien placee.", marginX, lineY, textColor);
    lineY += 30;
    drawText(renderer, font, "- Lettre jaune signifie", marginX, lineY, textColor);
    lineY += 30;
    drawText(renderer, font, "  presence, mais mal placee.", marginX, lineY, textColor);
    lineY += 30;
    drawText(renderer, font, "- Saisissez exactement", marginX, lineY, textColor);
    lineY += 30;
    drawText(renderer, font, "  le nombre de lettres requis.", marginX, lineY, textColor);
    lineY += 30;
    drawText(renderer, font, "- Vous avez 6 tentatives maximum.", marginX, lineY, textColor);
    SDL_Rect retourBtn = { popupX + popupW - 120, popupY + popupH - 50, 100, 30 };
    drawButton(renderer, font, retourBtn, "Retour", textColor, (SDL_Color){200,200,250,255});
    SDL_RenderPresent(renderer);
}

void renderMenu(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_SetRenderDrawColor(renderer, 200,200,250,255);
    SDL_RenderClear(renderer);
    SDL_Color textColor = {0,0,0,255};
    drawText(renderer, font, "Motus", 370, 50, textColor);
    SDL_Rect newGameBtn = {300,150,200,50}, continueBtn = {300,250,200,50}, exitBtn = {300,350,200,50}, helpBtn = {750,10,40,40};
    drawButton(renderer, font, newGameBtn, "Nouvelle Partie", textColor, (SDL_Color){100,200,100,255});
    drawButton(renderer, font, continueBtn, "Charger Partie", textColor, (SDL_Color){100,100,200,255});
    drawButton(renderer, font, exitBtn, "Quitter", textColor, (SDL_Color){200,100,100,255});
    drawButton(renderer, font, helpBtn, "?", textColor, (SDL_Color){220,220,220,255});
    SDL_RenderPresent(renderer);
}

void renderDifficulty(SDL_Renderer *renderer, TTF_Font *font, const char *pseudo, int activeField, Difficulty chosenDiff) {
    SDL_SetRenderDrawColor(renderer, 220,240,240,255);
    SDL_RenderClear(renderer);
    SDL_Color textColor = {0,0,0,255}, boxColor = {255,255,255,255}, activeColor = {180,255,180,255};
    drawText(renderer, font, "Entrez votre pseudo et choisissez une difficulte :", 150, 30, textColor);
    SDL_Rect pseudoBox = {250,100,300,40};
    SDL_SetRenderDrawColor(renderer, (activeField==1? activeColor.r : boxColor.r),
                            (activeField==1? activeColor.g : boxColor.g),
                            (activeField==1? activeColor.b : boxColor.b), 255);
    SDL_RenderFillRect(renderer, &pseudoBox);
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderDrawRect(renderer, &pseudoBox);
    int tw, th; TTF_SizeText(font, pseudo, &tw, &th);
    int textX = pseudoBox.x + (pseudoBox.w - tw) / 2;
    int textY = pseudoBox.y + (pseudoBox.h - th) / 2;
    drawText(renderer, font, pseudo, textX, textY, textColor);
    if (activeField == 1 && (SDL_GetTicks() % 1000 < 500)) {
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_Rect caret = {textX + tw, textY, 2, th};
        SDL_RenderFillRect(renderer, &caret);
    }
    SDL_Rect backBtn = {20,20,100,40};
    drawButton(renderer, font, backBtn, "Retour", textColor, (SDL_Color){220,220,220,255});
    SDL_Rect diffBtn1 = {130,180,120,40}, diffBtn2 = {270,180,120,40}, diffBtn3 = {410,180,120,40}, diffBtn4 = {550,180,120,40};
    drawButton(renderer, font, diffBtn1, "Debutant", textColor, (chosenDiff==DIFF_DEBUTANT? (SDL_Color){180,255,180,255} : (SDL_Color){220,220,220,255}));
    drawButton(renderer, font, diffBtn2, "Amateur", textColor, (chosenDiff==DIFF_AMATEUR? (SDL_Color){180,255,180,255} : (SDL_Color){220,220,220,255}));
    drawButton(renderer, font, diffBtn3, "Confirme", textColor, (chosenDiff==DIFF_CONFIRME? (SDL_Color){180,255,180,255} : (SDL_Color){220,220,220,255}));
    drawButton(renderer, font, diffBtn4, "Expert", textColor, (chosenDiff==DIFF_EXPERT? (SDL_Color){180,255,180,255} : (SDL_Color){220,220,220,255}));
    SDL_Rect startBtn = {300,260,200,50};
    drawButton(renderer, font, startBtn, "Commencer", textColor, (SDL_Color){150,150,250,255});
    SDL_RenderPresent(renderer);
}

void renderGame(SDL_Renderer *renderer, TTF_Font *font, const MotusGame *game, const Player *player, const char *currentGuess) {
    SDL_SetRenderDrawColor(renderer, 250,250,240,255);
    SDL_RenderClear(renderer);
    SDL_Color textColor = {0,0,0,255};
    int secretLen = (int)strlen(game->secretWord);
    char info[150];
    snprintf(info, sizeof(info), "Joueur : %s | Score : %d | Mot a deviner : %d lettres | Tentatives : %d/%d",
             player->pseudo, player->score, secretLen, game->attemptCount, MAX_ATTEMPTS);
    drawText(renderer, font, info, 20, 10, textColor);
    int startX = 100, startY = 60, cellSize = 50, spacing = 10;
    for (int i = 0; i < game->attemptCount; i++) {
        const char *guess = game->guesses[i];
        int len = (int)strlen(game->secretWord);
        SDL_Color letterColors[8];
        computeLetterColors(game->secretWord, guess, letterColors);
        for (int j = 0; j < len; j++) {
            SDL_Rect cellRect = { startX + j*(cellSize+spacing), startY + i*(cellSize+spacing), cellSize, cellSize };
            SDL_SetRenderDrawColor(renderer, letterColors[j].r, letterColors[j].g, letterColors[j].b, letterColors[j].a);
            SDL_RenderFillRect(renderer, &cellRect);
            SDL_SetRenderDrawColor(renderer, 0,0,0,255);
            SDL_RenderDrawRect(renderer, &cellRect);
            char letterStr[2] = { guess[j], '\0' };
            int offsetX = cellRect.x + (cellSize - 20) / 2;
            int offsetY = cellRect.y + (cellSize - 20) / 2;
            drawText(renderer, font, letterStr, offsetX, offsetY, textColor);
        }
    }
    SDL_Rect guessBox = {50, WINDOW_HEIGHT - 130, 300, 40};
    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
    SDL_RenderFillRect(renderer, &guessBox);
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderDrawRect(renderer, &guessBox);
    int tw, th; TTF_SizeText(font, currentGuess, &tw, &th);
    int guessX = guessBox.x + (guessBox.w - tw) / 2;
    int guessY = guessBox.y + (guessBox.h - th) / 2;
    drawText(renderer, font, currentGuess, guessX, guessY, textColor);
    if (!game->finished && (SDL_GetTicks() % 1000 < 500)) {
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_Rect caret = {guessX + tw, guessY, 2, th};
        SDL_RenderFillRect(renderer, &caret);
    }
    SDL_Rect validateBtn = {380, WINDOW_HEIGHT - 130, 150, 40};
    SDL_Rect saveBtn     = {550, WINDOW_HEIGHT - 130, 100, 40};
    SDL_Rect menuBtn     = {670, WINDOW_HEIGHT - 130, 100, 40};
    if (!game->finished) drawButton(renderer, font, validateBtn, "Valider", textColor, (SDL_Color){150,250,150,255});
    else drawButton(renderer, font, validateBtn, "Termine", textColor, (SDL_Color){200,200,200,255});
    drawButton(renderer, font, saveBtn, "Sauver", textColor, (SDL_Color){250,250,150,255});
    drawButton(renderer, font, menuBtn, "Menu", textColor, (SDL_Color){250,150,150,255});
    SDL_RenderPresent(renderer);
}

void renderGameOver(SDL_Renderer *renderer, TTF_Font *font, const MotusGame *game, const Player *player) {
    SDL_SetRenderDrawColor(renderer, 255,230,230,255);
    SDL_RenderClear(renderer);
    SDL_Color textColor = {0,0,0,255};
    int titleWidth = 200, titleX = (WINDOW_WIDTH - titleWidth) / 2;
    drawText(renderer, font, "Fin de la partie", titleX, 100, textColor);
    char msg[200];
    if (game->success)
        snprintf(msg, sizeof(msg), "Bravo %s ! Vous avez trouve le mot %s en %d tentatives !",
                 player->pseudo, game->secretWord, game->attemptCount);
    else
        snprintf(msg, sizeof(msg), "Dommage %s, le mot etait %s.", player->pseudo, game->secretWord);
    int msgX = (WINDOW_WIDTH - 600) / 2;
    drawText(renderer, font, msg, msgX, 180, textColor);
    SDL_Rect menuBtn = {300,300,200,50};
    drawButton(renderer, font, menuBtn, "Retour Menu", textColor, (SDL_Color){180,180,250,255});
    SDL_RenderPresent(renderer);
}
