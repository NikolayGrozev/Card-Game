#include <stdio.h>
#include <string.h>

typedef struct Card {
    char power;
    char suit;
} Card;

int decrypt_single_card(FILE *file, const char *key, int cardIndex, Card *drawnCard) {

    unsigned int hexP, hexS;

    //%02X%02X търси 2 шестнадесетични числа долепени едно до друго

    if (fscanf(file, "%02X%02X ", &hexP, &hexS) == 2) {

        int keyLen = strlen(key);
        
        int pIndex = (cardIndex * 2) % keyLen;
        int sIndex = (cardIndex * 2 + 1) % keyLen;

        drawnCard->power = ((unsigned char)hexP) ^ key[pIndex];
        drawnCard->suit = ((unsigned char)hexS) ^ key[sIndex];
        
        return 1;
    }
    return 0;
}

int main()
{
    FILE *file = fopen("deck.txt", "r");
    if (!file) {
        printf("Error opening file %s\n", file);
        return 0;
    }

    Card memory_array[52];
    int count = 0;

    while (decrypt_single_card(file, "secretKey", count, &memory_array[count])) {
        count++;
    }

    printf("%d\n", count);

    fclose(file);

    return 0;
}