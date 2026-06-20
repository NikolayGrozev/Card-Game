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
    int key_len = strlen(key);
    int cards_count = 0;
    
    // Четем, криптираме и записваме във временния файл
    while (fscanf(file, "%c %c ", &power, &suit) == 2) {
        unsigned char enc_p = power ^ key[0 % key_len];
        unsigned char enc_s = suit ^ key[1 % key_len];
        fprintf(tempFile, "%02X%02X ", enc_p, enc_s);
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