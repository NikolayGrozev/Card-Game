#include "functions.h"

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
    
    char power, suit;
    int keyLen = strlen(key);
    int cards_count = 0;
    
    // Четем, криптираме и записваме във временния файл
    while (fscanf(file, "%c %c ", &power, &suit) == 2) {
        int pIndex = (cards_count * 2) % keyLen;
        int sIndex = (cards_count * 2 + 1) % keyLen;
        unsigned char pEnc = power ^ key[pIndex];
        unsigned char sEnc = suit ^ key[sIndex];
        fprintf(tempFile, "%02X%02X ", pEnc, sEnc);
        //Превръща число в 16-на броена с-ма в 2 символа. Ако е 1 символ първият символ става 0
        cards_count++;
    }
    
    fclose(file);
    fclose(tempFile);
    
    // Заместваме оригиналния файл
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
    
    Card currentCard; // Създаваме структура, в която ще записваме декриптираната карта
    int cards_count = 0;
    
    // Подаваме файла, ключа, текущия брой (като индекс) и адреса на currentCard
    while (decrypt_single_card(file, key, cards_count, &currentCard)) {
        // Записваме декриптираната сила и боя във временния файл
        fprintf(tempFile, "%c %c ", currentCard.power, currentCard.suit);
        cards_count++;
    }
    
    fclose(file);
    fclose(tempFile);
    
    remove(filename);
    rename(temp_filename, filename);
    
    printf("Файлът '%s' беше успешно декриптиран (%d карти).\n", filename, cards_count);
}

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
        char power, suit;
        while (fscanf(file, "%c %c ", &power, &suit) == 2) {
            count++;
        }
    }
    
    fclose(file);
    return count;
}