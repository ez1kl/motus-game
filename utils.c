#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
int loadDictionary(const char *filename, char wordList[][8], int maxWords) {
    FILE *f = fopen(filename, "r");
    if (!f) { printf("Erreur ouverture %s\n", filename); return 0; }
    int count = 0;
    while (!feof(f) && count < maxWords) {
        if (fscanf(f, "%7s", wordList[count]) == 1) {
            toUpperCase(wordList[count]);
            count++;
        }
    }
    fclose(f);
    return count;
}
bool pickRandomWord(char wordList[][8], int wordCount, int length, char outWord[8]) {
    char candidates[1000][8];
    int cCount = 0;
    for (int i = 0; i < wordCount; i++) {
        if ((int)strlen(wordList[i]) == length) {
            strcpy(candidates[cCount], wordList[i]);
            cCount++;
        }
    }
    if (cCount == 0) return false;
    int index = rand() % cCount;
    strcpy(outWord, candidates[index]);
    return true;
}
void toUpperCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = (char)toupper((unsigned char)str[i]);
    }
}
