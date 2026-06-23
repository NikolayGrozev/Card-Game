#include "functions."

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

    FILE *file = fopen(filename, "r");
    free(filename);
    if (!file) {
        printf("Error opening file %s\n", file);
        return 0;
    }

    printf("\nOpened File sucessfully ");


    Card memory_array[52];
    int count = 0;

    while (decrypt_single_card(file, "secretKey", count, &memory_array[count])) {
        count++;
    }

    printf("%d\n", count);

    fclose(file);

    return 0;
}