#include "functions.h"

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

int main()
{
    char * filename = InputFileName();
    const char * secretKey = "secretKey";

    int choice = 0;

    while (1) {
        int status = is_file_encrypted(filename);
            
        if (status == -1) {
            printf("\nThe file %s did not open.\n", filename);
            break;
        } 
        else if (status == -2) {
            printf("\nThe file %d is empty or has invalid data\n", filename);
            break; 
        }

        int cards_count = count_cards(filename, status);

        if (cards_count != 52) {
            printf("\nThe file must have 52 cards. It has %d cards.\n", cards_count);
            break; 
        }

        printf("\n========== Menu ==========\n");
        printf("File: %s (Cards: %d, Status: %s)\n", filename, cards_count, status == 1 ? "Encrypted" : "Not encrypted");
        printf("--------------------------\n");
        printf("1. encrypt the file\n");
        printf("2. decrypt the file\n");
        printf("3. Play game\n");
        printf("4. Exit\n");
        printf("==========================\n");
        printf("Enter option: ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Please enter a valid number.\n");
            continue;
        }

        if (choice == 1) {
            if (status == 1) {
                printf("\nThe file is already encrypted.\n");
            } else {
                printf("\nEncrypting... \n");
                encryptDeck(filename, secretKey);
            }
        } 
        else if (choice == 2) {
            if (status == 0) {
                printf("\nThe file is not encrypted.\n");
            } else {
                printf("\nDecrypting... \n");
                decryptDeck(filename, secretKey);
            }
        } 
        else if (choice == 3) {
            if (status == 0) {
                printf("\nThe file must be encrypted to start the game\n");
            } else {
                printf("\nStarting game... \n");
                //Тук пиши играта!
            }
        } 
        else if (choice == 4) {
            printf("\nExiting... \n");
            break;
        } 
        else {
            printf("\nInvalid option. Choose from 1 to 4.\n");
        }
    }

    free(filename);
    return 0;
}