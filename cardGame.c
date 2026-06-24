#include "functions.h"

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
        } else if (status == -2) {
            printf("\nThe file %s is empty or has invalid data\n", filename);
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
        printf("Enter option(1-4): ");
        
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
                
                FILE * gameFile = fopen(filename, "r");
                if(!gameFile){
                    printf("\nError opening file for the game.\n");
                    continue;
                }

                Card playerCards[4][13];
                int cardsAmmountPerPlayer = 0;
                int cardsInGame = 0;

                int winnersQueue[4] = {0, 0, 0, 0};
                int winningValues[4] = {0, 0, 0, 0}; 
                char noWinners = 1;
                
                for(int i = 0; i < 13; i++){
                    for(int j = 0; j < 4; j++){
                        decrypt_single_card(gameFile, secretKey, cardsInGame, &playerCards[j][cardsAmmountPerPlayer]);
                        cardsInGame++;
                    }
                    cardsAmmountPerPlayer++;
                    
                    for(int j = 0; j < 4; j++){
                        int winVal = hasWon(cardsAmmountPerPlayer, playerCards[j]);
                        if(winVal > 0){
                            winnersQueue[j] = 1;
                            winningValues[j] = winVal;
                            noWinners = 0;
                        }
                    }
                    if(!noWinners){
                        break; 
                    }
                }

                fclose(gameFile);

                int winnerIndex = 0;

                if(noWinners){
                    
                    winnerIndex = getFinalWinner_ByHighCard(cardsAmmountPerPlayer, playerCards);
                } else {
                    winnerIndex = getFinalWinner_ByComparrison(winnersQueue, winningValues, cardsAmmountPerPlayer, playerCards);
                }

                printf("\nPlayer %d wins: ", winnerIndex + 1);
                printPlayer(cardsAmmountPerPlayer, playerCards[winnerIndex]);
                printf("\n");
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