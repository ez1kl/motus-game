#ifndef GAME_H
#define GAME_H
#include "common.h"
void getFeedback(const char *secret, const char *guess, int *blacks, int *whites);
void initGame(MotusGame *game, Difficulty diff, const char *secretChosen);
void processAttempt(MotusGame *game, const char *attempt);
bool saveGame(const Player *player, const MotusGame *game);
bool loadGame(Player *player, MotusGame *game);
int loadPlayerScore(const char *pseudo);
void savePlayerScore(const char *pseudo, int score);
void updatePlayerScore(Player *player, int attempts);
#endif
