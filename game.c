#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void getFeedback(const char *secret, const char *guess, int *blacks, int *whites) {
    *blacks = 0; *whites = 0;
    int freqSecret[26] = {0}, freqGuess[26] = {0};
    int len = (int)strlen(secret);
    for (int i = 0; i < len; i++) {
        if (secret[i] == guess[i])
            (*blacks)++;
        else {
            freqSecret[secret[i] - 'A']++;
            freqGuess[guess[i] - 'A']++;
        }
    }
    for (int i = 0; i < 26; i++) {
        *whites += (freqSecret[i] < freqGuess[i] ? freqSecret[i] : freqGuess[i]);
    }
}
void initGame(MotusGame *game, Difficulty diff, const char *secretChosen) {
    game->difficulty = diff;
    strncpy(game->secretWord, secretChosen, sizeof(game->secretWord)-1);
    game->secretWord[sizeof(game->secretWord)-1] = '\0';
    game->attemptCount = 0;
    game->finished = false;
    game->success = false;
    for (int i = 0; i < MAX_ATTEMPTS; i++) {
        game->guesses[i][0] = '\0';
        game->blacks[i] = 0;
        game->whites[i] = 0;
    }
}
void processAttempt(MotusGame *game, const char *attempt) {
    if (game->finished) return;
    int idx = game->attemptCount;
    strncpy(game->guesses[idx], attempt, sizeof(game->guesses[idx])-1);
    game->guesses[idx][sizeof(game->guesses[idx])-1] = '\0';
    getFeedback(game->secretWord, attempt, &game->blacks[idx], &game->whites[idx]);
    game->attemptCount++;
    if (game->blacks[idx] == (int)strlen(game->secretWord)) { game->finished = true; game->success = true; }
    else if (game->attemptCount >= MAX_ATTEMPTS) { game->finished = true; }
}
bool saveGame(const Player *player, const MotusGame *game) {
    FILE *f = fopen("save.txt", "w");
    if (!f) return false;
    fprintf(f, "%s\n%d\n", player->pseudo, player->score);
    fprintf(f, "%d\n", game->difficulty);
    fprintf(f, "%s\n", game->secretWord);
    fprintf(f, "%d\n", game->attemptCount);
    fprintf(f, "%d\n", game->finished);
    fprintf(f, "%d\n", game->success);
    for (int i = 0; i < game->attemptCount; i++) {
        fprintf(f, "%s %d %d\n", game->guesses[i], game->blacks[i], game->whites[i]);
    }
    fclose(f);
    return true;
}
bool loadGame(Player *player, MotusGame *game) {
    FILE *f = fopen("save.txt", "r");
    if (!f) return false;
    fscanf(f, "%49s\n%d\n", player->pseudo, &player->score);
    int diff;
    fscanf(f, "%d\n", &diff);
    game->difficulty = (Difficulty)diff;
    fscanf(f, "%7s\n", game->secretWord);
    fscanf(f, "%d\n", &game->attemptCount);
    int finished, success;
    fscanf(f, "%d\n", &finished);
    fscanf(f, "%d\n", &success);
    game->finished = finished;
    game->success = success;
    for (int i = 0; i < game->attemptCount; i++) {
        fscanf(f, "%7s %d %d\n", game->guesses[i], &game->blacks[i], &game->whites[i]);
    }
    fclose(f);
    return true;
}
int loadPlayerScore(const char *pseudo) {
    FILE *f = fopen("scores.txt", "r");
    char name[50];
    int score, foundScore = 0;
    if (f) {
        while (fscanf(f, "%49s %d", name, &score) == 2) {
            if (strcmp(name, pseudo) == 0)
                foundScore = score;
        }
        fclose(f);
    }
    return foundScore;
}
void savePlayerScore(const char *pseudo, int score) {
    FILE *f = fopen("scores.txt", "a");
    if (f) { fprintf(f, "%s %d\n", pseudo, score); fclose(f); }
}
void updatePlayerScore(Player *player, int attempts) {
    int oldScore = loadPlayerScore(player->pseudo);
    int newScore = oldScore + 10;
    player->score = newScore;
    savePlayerScore(player->pseudo, newScore);
}
