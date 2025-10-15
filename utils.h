#ifndef UTILS_H
#define UTILS_H
#include "common.h"
int loadDictionary(const char *filename, char wordList[][8], int maxWords);
bool pickRandomWord(char wordList[][8], int wordCount, int length, char outWord[8]);
void toUpperCase(char *str);
#endif
