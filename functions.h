#ifndef GUARD
#define GUARD

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Card {
    char power;
    char suit;
} Card;

void encryptDeck(const char* filename, const char* key);
int decrypt_single_card(FILE *file, const char *key, int cardIndex, Card *drawnCard);

#endif