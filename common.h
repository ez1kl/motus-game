#ifndef COMMON_H
#define COMMON_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define MAX_ATTEMPTS 6
typedef enum { DIFF_NONE = 0, DIFF_DEBUTANT = 4, DIFF_AMATEUR = 5, DIFF_CONFIRME = 6, DIFF_EXPERT = 7 } Difficulty;
typedef enum { STATE_MENU, STATE_HELP, STATE_DIFFICULTY, STATE_GAME, STATE_GAME_OVER, STATE_EXIT } AppState;
typedef struct { char pseudo[50]; int score; } Player;
typedef struct { Difficulty difficulty; char secretWord[8]; char guesses[MAX_ATTEMPTS][8]; int blacks[MAX_ATTEMPTS]; int whites[MAX_ATTEMPTS]; int attemptCount; bool finished; bool success; } MotusGame;
#endif
