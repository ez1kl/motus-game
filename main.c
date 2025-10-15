#define SDL_MAIN_HANDLED
#include "common.h"
#include "game.h"
#include "utils.h"
#include "render.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MAX_WORDS 1000
static bool pointInRect(int x, int y, SDL_Rect rect){ return (x>=rect.x && x<rect.x+rect.w && y>=rect.y && y<rect.y+rect.h); }
int main(int argc, char *argv[]){
    (void)argc; (void)argv;
    srand((unsigned int)time(NULL));
    if(SDL_Init(SDL_INIT_VIDEO)!=0){ printf("Erreur SDL_Init: %s\n", SDL_GetError()); return 1; }
    if(TTF_Init()!=0){ printf("Erreur TTF_Init: %s\n", TTF_GetError()); SDL_Quit(); return 1; }
    SDL_Window *window = SDL_CreateWindow("Motus", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if(!window){ printf("Erreur SDL_CreateWindow: %s\n", SDL_GetError()); TTF_Quit(); SDL_Quit(); return 1; }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){ printf("Erreur SDL_CreateRenderer: %s\n", SDL_GetError()); SDL_DestroyWindow(window); TTF_Quit(); SDL_Quit(); return 1; }
    TTF_Font *font = TTF_OpenFont("font.ttf", 24);
    if(!font){ printf("Erreur TTF_OpenFont: %s\n", TTF_GetError()); SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window); TTF_Quit(); SDL_Quit(); return 1; }
    SDL_Cursor *cursorDefault = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    SDL_Cursor *cursorHand = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    char wordList[MAX_WORDS][8];
    int wordCount = loadDictionary("dictionary.txt", wordList, MAX_WORDS);
    if(wordCount==0){ printf("Aucun mot charge. Verifiez dictionary.txt\n"); }
    AppState state = STATE_MENU;
    bool running = true;
    SDL_Event e;
    Difficulty chosenDiff = DIFF_NONE;
    char pseudo[50] = "";
    int activeField = 1;
    SDL_StartTextInput();
    Player player; strcpy(player.pseudo, ""); player.score = 0;
    MotusGame game; memset(&game, 0, sizeof(game));
    char currentGuess[8] = "";
    while(running){
        int mouseX, mouseY; SDL_GetMouseState(&mouseX, &mouseY);
        bool overButton = false;
        if(state==STATE_MENU){
            SDL_Rect newGameBtn = {300,150,200,50}, continueBtn = {300,250,200,50}, exitBtn = {300,350,200,50}, helpBtn = {750,10,40,40};
            if(pointInRect(mouseX,mouseY,newGameBtn) || pointInRect(mouseX,mouseY,continueBtn) || pointInRect(mouseX,mouseY,exitBtn) || pointInRect(mouseX,mouseY,helpBtn))
                overButton = true;
        }
        else if(state==STATE_HELP){
            int popupW = 700, popupH = 450, popupX = (WINDOW_WIDTH-popupW)/2, popupY = (WINDOW_HEIGHT-popupH)/2;
            SDL_Rect retourBtn = {popupX+popupW-110, popupY+popupH-50,90,30};
            if(pointInRect(mouseX,mouseY,retourBtn))
                overButton = true;
        }
        else if(state==STATE_DIFFICULTY){
            SDL_Rect pseudoBox = {250,100,300,40}, backBtn = {20,20,100,40}, diffBtn1 = {130,180,120,40}, diffBtn2 = {270,180,120,40}, diffBtn3 = {410,180,120,40}, diffBtn4 = {550,180,120,40}, startBtn = {300,260,200,50};
            if(pointInRect(mouseX,mouseY,pseudoBox) || pointInRect(mouseX,mouseY,backBtn) || pointInRect(mouseX,mouseY,diffBtn1) || pointInRect(mouseX,mouseY,diffBtn2) || pointInRect(mouseX,mouseY,diffBtn3) || pointInRect(mouseX,mouseY,diffBtn4) || pointInRect(mouseX,mouseY,startBtn))
                overButton = true;
        }
        else if(state==STATE_GAME){
            SDL_Rect validateBtn = {380, WINDOW_HEIGHT-130,150,40}, saveBtn = {550, WINDOW_HEIGHT-130,100,40}, menuBtn = {670, WINDOW_HEIGHT-130,100,40};
            if(pointInRect(mouseX,mouseY,validateBtn) || pointInRect(mouseX,mouseY,saveBtn) || pointInRect(mouseX,mouseY,menuBtn))
                overButton = true;
        }
        else if(state==STATE_GAME_OVER){
            SDL_Rect menuBtn = {300,300,200,50};
            if(pointInRect(mouseX,mouseY,menuBtn))
                overButton = true;
        }
        SDL_SetCursor(overButton ? cursorHand : cursorDefault);
        while(SDL_PollEvent(&e)){
            if(e.type==SDL_QUIT){ state = STATE_EXIT; running = false; }
            if(state==STATE_HELP){
                if(e.type==SDL_MOUSEBUTTONDOWN){
                    int mx = e.button.x, my = e.button.y;
                    int popupW = 700, popupH = 450, popupX = (WINDOW_WIDTH-popupW)/2, popupY = (WINDOW_HEIGHT-popupH)/2;
                    SDL_Rect retourBtn = {popupX+popupW-110, popupY+popupH-50,90,30};
                    if(pointInRect(mx,my,retourBtn))
                        state = STATE_MENU;
                }
                continue;
            }
            if(e.type==SDL_TEXTINPUT){
                if(state==STATE_DIFFICULTY && activeField==1){
                    if(strlen(pseudo) < sizeof(pseudo)-1)
                        strcat(pseudo,e.text.text);
                }
                else if(state==STATE_GAME && !game.finished){
                    if(strlen(currentGuess) < strlen(game.secretWord)){
                        strcat(currentGuess,e.text.text);
                        toUpperCase(currentGuess);
                    }
                }
            }
            if(e.type==SDL_KEYDOWN){
                if(e.key.keysym.sym==SDLK_BACKSPACE){
                    if(state==STATE_DIFFICULTY && activeField==1){
                        int len = strlen(pseudo);
                        if(len>0) pseudo[len-1]='\0';
                    }
                    else if(state==STATE_GAME && !game.finished){
                        int len = strlen(currentGuess);
                        if(len>0) currentGuess[len-1]='\0';
                    }
                }
            }
            if(e.type==SDL_MOUSEBUTTONDOWN){
                int mx = e.button.x, my = e.button.y;
                if(state==STATE_MENU){
                    SDL_Rect newGameBtn = {300,150,200,50}, continueBtn = {300,250,200,50}, exitBtn = {300,350,200,50}, helpBtn = {750,10,40,40};
                    if(pointInRect(mx,my,newGameBtn)){ chosenDiff = DIFF_NONE; memset(pseudo,0,sizeof(pseudo)); state = STATE_DIFFICULTY; }
                    else if(pointInRect(mx,my,continueBtn)){
                        if(loadGame(&player,&game)){ state = STATE_GAME; memset(currentGuess,0,sizeof(currentGuess)); }
                        else { printf("Aucune partie sauvegardee.\n"); }
                    }
                    else if(pointInRect(mx,my,exitBtn)){ state = STATE_EXIT; running = false; }
                    else if(pointInRect(mx,my,helpBtn)){ state = STATE_HELP; }
                }
                else if(state==STATE_DIFFICULTY){
                    SDL_Rect pseudoBox = {250,100,300,40}, backBtn = {20,20,100,40}, diffBtn1 = {130,180,120,40}, diffBtn2 = {270,180,120,40}, diffBtn3 = {410,180,120,40}, diffBtn4 = {550,180,120,40}, startBtn = {300,260,200,50};
                    if(pointInRect(mx,my,pseudoBox)) activeField = 1;
                    if(pointInRect(mx,my,backBtn)) state = STATE_MENU;
                    if(pointInRect(mx,my,diffBtn1)) chosenDiff = DIFF_DEBUTANT;
                    else if(pointInRect(mx,my,diffBtn2)) chosenDiff = DIFF_AMATEUR;
                    else if(pointInRect(mx,my,diffBtn3)) chosenDiff = DIFF_CONFIRME;
                    else if(pointInRect(mx,my,diffBtn4)) chosenDiff = DIFF_EXPERT;
                    if(pointInRect(mx,my,startBtn)){
                        if(strlen(pseudo)==0 || chosenDiff==DIFF_NONE){ printf("Veuillez saisir un pseudo et choisir une difficulte.\n"); }
                        else { strcpy(player.pseudo, pseudo); player.score = loadPlayerScore(player.pseudo); char chosenWord[8] = ""; if(!pickRandomWord(wordList, wordCount, (int)chosenDiff, chosenWord)){ printf("Aucun mot de %d lettres dans le dictionnaire.\n",(int)chosenDiff); state = STATE_MENU; } else { initGame(&game, chosenDiff, chosenWord); memset(currentGuess,0,sizeof(currentGuess)); state = STATE_GAME; } }
                    }
                }
                else if(state==STATE_GAME){
                    SDL_Rect validateBtn = {380, WINDOW_HEIGHT-130,150,40}, saveBtn = {550, WINDOW_HEIGHT-130,100,40}, menuBtn = {670, WINDOW_HEIGHT-130,100,40}, guessBox = {50, WINDOW_HEIGHT-130,300,40};
                    if(pointInRect(mx,my,validateBtn)){
                        if(!game.finished){
                            if((int)strlen(currentGuess)==(int)strlen(game.secretWord)){
                                processAttempt(&game,currentGuess);
                                memset(currentGuess,0,sizeof(currentGuess));
                                if(game.finished) state = STATE_GAME_OVER;
                            } else { printf("Votre tentative doit faire %d lettres.\n",(int)strlen(game.secretWord)); }
                        }
                    }
                    if(pointInRect(mx,my,saveBtn)){ if(saveGame(&player,&game)) printf("Partie sauvegardee.\n"); }
                    if(pointInRect(mx,my,menuBtn)){ state = STATE_MENU; }
                }
                else if(state==STATE_GAME_OVER){
                    SDL_Rect menuBtn = {300,300,200,50};
                    if(pointInRect(mx,my,menuBtn)) state = STATE_MENU;
                }
            }
        }
        if(state==STATE_MENU) renderMenu(renderer, font);
        else if(state==STATE_HELP) renderHelpPopup(renderer, font);
        else if(state==STATE_DIFFICULTY) renderDifficulty(renderer, font, pseudo, activeField, chosenDiff);
        else if(state==STATE_GAME) renderGame(renderer, font, &game, &player, currentGuess);
        else if(state==STATE_GAME_OVER) renderGameOver(renderer, font, &game, &player);
        SDL_Delay(16);
    }
    SDL_StopTextInput();
    SDL_FreeCursor(cursorDefault);
    SDL_FreeCursor(cursorHand);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
