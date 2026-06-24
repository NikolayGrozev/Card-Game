#include "functions.h"

int color_to_int(char color){
    if(color == 'C'){
        return 1;
    }
    if(color == 'D'){
        return 2;
    }
    if(color == 'H'){
        return 3;
    }
    if(color == 'S'){
        return 4;
    }

    printf("\nInvalid Input");
    exit(1);
}

char * InputFileName() {
    int size = 4;
    char * c = (char *)calloc(size, sizeof(char));
    
    if (c == NULL) {
        printf("\nError in allocating memory");
        exit(1);
    }

    printf("\nEnter file name: ");

    int count = 0;
    int ch; 

    while ((ch = getchar()) != '\n' && ch != EOF) {
        
        if (count == size - 1) {
            size *= 2;
            
            char * temp = (char *)realloc(c, size * sizeof(char));
            if (temp == NULL) {
                free(c);
                printf("\nError in reallocating memory");
                exit(1);
            }
            c = temp;
        }
        
        c[count++] = (char)ch;
    }

    c[count] = '\0';

    c = (char *)realloc(c, (count + 1) * sizeof(char));
    if (c == NULL) {
        printf("\nError in reallocating memory");
        exit(1);
    }

    return c;
}

int hasWon(int size, Card * cards) {

    int cardValues[13] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 15, 20};
    int currCount = 0;

    for (int i = 0; i < 13; i++) {
        currCount = 0;
        for (int j = 0; j < size; j++) {
            if (charVal_to_int(cards[j].value) == cardValues[i]) {
                currCount++;
            }
        }
        if (currCount == 4) {
            return cardValues[i];
        }
    }
    return 0;
}

int charVal_to_int(char c) {
    if (c >= '1' && c <= '9') {
        return c - '0';
    }
    if (c == 'J'){
        return 10;
    } 
    if (c == 'Q'){
        return 12;
    }
    if (c == 'K') {
        return 15;
    }
    if (c == 'A'){
        return 20;
    }
    
    printf("\nInvalid Argument");
    exit(1);
}

void printPlayer(int size, Card * cards) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            
            int val1 = charVal_to_int(cards[j].value);
            int val2 = charVal_to_int(cards[j + 1].value);
            
            char shouldSwap = 0; 
            
            if (val2 > val1) {
                shouldSwap = 1;
            } else if (val2 == val1) {
                int col1 = color_to_int(cards[j].color);
                int col2 = color_to_int(cards[j + 1].color);
                
                if (col2 > col1) {
                    shouldSwap = 1;
                }
            }
            
            if (shouldSwap) {
                Card temp = cards[j];
                cards[j] = cards[j + 1];
                cards[j + 1] = temp;
            }
        }
    }
    
    for (int i = 0; i < size; i++) {
        printf("(%c, %c) ",cards[i].value, cards[i].color);
    }
}

int getFinalWinner_ByComparrison(int winnersQueue[4], int winningValues[4], int size, Card playerCards[4][13]) {
    int biggestValue = 0;
    int biggestValueIndex = 0;
    int maxSumColors = 0;

    for (int i = 0; i < 4; i++) {
        if (!winnersQueue[i]) continue;

        if (winningValues[i] > biggestValue) {
            biggestValue = winningValues[i];
            biggestValueIndex = i;

            int sumColors = 0;
            for (int j = 0; j < size; j++) {
                if (charVal_to_int(playerCards[i][j].value) == biggestValue) {
                    sumColors += color_to_int(playerCards[i][j].color);
                }
            }
            maxSumColors = sumColors;

        } else if (winningValues[i] == biggestValue) {
            int sumColors = 0;
            for (int j = 0; j < size; j++) {
                if (charVal_to_int(playerCards[i][j].value) == biggestValue) {
                    sumColors += color_to_int(playerCards[i][j].color);
                }
            }
            if (sumColors > maxSumColors) {
                maxSumColors = sumColors;
                biggestValueIndex = i;
            }
        }
    }
    return biggestValueIndex;
}

int getFinalWinner_ByHighCard(int size, Card playerCards[4][13]) {
    int biggestValue = 0;
    int biggestValueIndex = 0;
    int maxSumColors = 0;
    
    for (int i = 0; i < 4; i++) {
        int sum = 0;
        int sumColors = 0;
        
        for (int j = 0; j < size; j++) {
            sum += charVal_to_int(playerCards[i][j].value);
            sumColors += color_to_int(playerCards[i][j].color);
        }

        if (sum > biggestValue) {
            biggestValue = sum;
            maxSumColors = sumColors;
            biggestValueIndex = i;
        } else if (sum == biggestValue) {
            if (sumColors > maxSumColors) {
                biggestValue = sum;
                maxSumColors = sumColors;
                biggestValueIndex = i;
            }
        }
    }
    return biggestValueIndex;
}

void encryptDeck(const char* filename, const char* key) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file %s\n", filename);
        return;
    }
    
    const char* temp_filename = "temp_encrypted_deck.txt";
    FILE *tempFile = fopen(temp_filename, "w");
    if (!tempFile) {
        printf("Error opening file %s\n", temp_filename);
        fclose(file);
        return;
    }
    
    char value, color;
    int keyLen = strlen(key);
    int cards_count = 0;
    
    while (fscanf(file, "%c %c ", &value, &color) == 2) {
        int pIndex = (cards_count * 2) % keyLen;
        int sIndex = (cards_count * 2 + 1) % keyLen;
        unsigned char pEnc = value ^ key[pIndex];
        unsigned char sEnc = color ^ key[sIndex];
        fprintf(tempFile, "%02X%02X ", pEnc, sEnc);
        //Превръща число в 16-на броена с-ма в 2 символа. Ако е 1 символ първият символ става 0
        cards_count++;
    }
    
    fclose(file);
    fclose(tempFile);
    
    remove(filename);
    rename(temp_filename, filename);
    
    printf("The file '%s' was encrypted (%d cards)\n", filename, cards_count);
}

void decryptDeck(const char* filename, const char* key) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file %s\n", filename);
        return;
    }
    
    const char* temp_filename = "temp_decrypted_deck.txt";
    FILE *tempFile = fopen(temp_filename, "w");
    if (!tempFile) {
        printf("Error opening temp file\n");
        fclose(file);
        return;
    }
    
    Card currentCard;
    int cards_count = 0;
    
    while (decrypt_single_card(file, key, cards_count, &currentCard)) {
        fprintf(tempFile, "%c %c ", currentCard.value, currentCard.color);
        cards_count++;
    }
    
    fclose(file);
    fclose(tempFile);
    
    remove(filename);
    rename(temp_filename, filename);
    
    printf("The file %s was decrypted (%d cards).\n", filename, cards_count);
}

int decrypt_single_card(FILE *file, const char *key, int cardIndex, Card *drawnCard) {

    unsigned int hexP, hexS;

    //%02X%02X търси 2 шестнадесетични числа долепени едно до друго

    if (fscanf(file, "%02X%02X ", &hexP, &hexS) == 2) {

        int keyLen = strlen(key);
        
        int pIndex = (cardIndex * 2) % keyLen;
        int sIndex = (cardIndex * 2 + 1) % keyLen;

        drawnCard->value = ((unsigned char)hexP) ^ key[pIndex];
        drawnCard->color = ((unsigned char)hexS) ^ key[sIndex];
        
        return 1;
    }
    return 0;
}

int is_file_encrypted(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file %s\n", filename);
        return -1;
    }

    char buffer[5];
    if (fscanf(file, "%4s", buffer) == 1) {
        fclose(file);
        if (strlen(buffer) == 4) {
            return 1;
        } else {
            return 0;
        }
    }
    fclose(file);
    return -2;
}

int count_cards(const char* filename, int is_encrypted) {
    FILE *file = fopen(filename, "r");
    if (!file) return 0;

    int count = 0;
    
    if (is_encrypted == 1) { 
        char buffer[5];
        while (fscanf(file, "%4s", buffer) == 1) {
            if (strlen(buffer) == 4) {
                count++;
            }
        }
    } else if (is_encrypted == 0) { 
        char value, color;
        while (fscanf(file, "%c %c ", &value, &color) == 2) {
            count++;
        }
    }
    
    fclose(file);
    return count;
}