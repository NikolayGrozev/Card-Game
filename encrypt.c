#include <stdio.h>
#include <string.h>

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
        cards_count++;
    }
    
    fclose(file);
    fclose(tempFile);
    
    // Заместваме оригиналния файл
    remove(filename);
    rename(temp_filename, filename);
    
    printf("The file '%s' was encrypted (%d cards)\n", filename, cards_count);
}

int main() {
    // Стартираме криптирането
    const char* target_file = "deck.txt";
    const char* my_key = "secretKey";
    
    encryptDeck(target_file, my_key);
    
    return 0;
}