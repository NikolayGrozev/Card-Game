#ifndef GUARD
#define GUARD

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Card {
    char value;
    char color;
} Card;

void encryptDeck(const char* filename, const char* key);
void decryptDeck(const char* filename, const char* key);
int decrypt_single_card(FILE *file, const char *key, int cardIndex, Card *drawnCard);
int is_file_encrypted(const char* filename);
int count_cards(const char* filename, int is_encrypted);

char * InputFileName();
int charVal_to_int(char c);
int hasWon(int size, Card * cards);
void printPlayer(int size, Card * cards);
int color_to_int(char color);

int getFinalWinner_ByHighCard(int size, Card playerCards[4][13]);
int getFinalWinner_ByComparrison(int winnersQueue[4], int winningValues[4], int size, Card playerCards[4][13]);

#endif