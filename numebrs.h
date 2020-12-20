#ifndef numebrs_h
#define numebrs_h

#include "config.h"
#include "errors.h"
#include <stdlib.h>

int* getFromFile(int srcNum){
    static int numbers[NUM_COUNT];
    
    FILE* fp;
    char arrName = 'A';
    int fileNumber = 0;
    
    switch (srcNum) {
        case F_SENDER:
            fileNumber = 2;
            arrName = 'A';
            break;
        case S_SENDER:
            arrName = 'B';
            error("Error getFromFile()");
            break;
        case T_SENDER:
            fileNumber = 4;
            arrName = 'C';
            break;
        default:
            break;
    }
    
    if(fileNumber == 0) error("Error getFromFile() file number");
    
    char fname[255];
    sprintf(fname, "file%d.txt", fileNumber);
    
    fp = fopen(fname, "r");
    
    int i;
    char numbersStr[11];
    
    printf("Read numbers from %s: ", fname);
    
    for (i = 0; i < NUM_COUNT; i++) {
        fgets(numbersStr, 11, fp);
        numbers[i] = atoi(numbersStr);
        printf(" %d", numbers[i]);
    }
    
    printf("\n");
    
    fclose(fp);
    
    return numbers;
}

int* getFromTerminal(int srcNum){
    static int numbers[NUM_COUNT];
    
    FILE* fp = stdin;
    char arrName = 'A';
    
    switch (srcNum) {
        case F_SENDER:
            arrName = 'A';
            break;
        case S_SENDER:
            arrName = 'B';
            break;
        case T_SENDER:
            arrName = 'C';
            break;
        default:
            break;
    }
    
    int i;
    char numbersStr[11];
    
    printf("Enter numbers for %c:\n", arrName);
    
    for (i = 0; i < NUM_COUNT; i++) {
        printf("%c[%d] = ", arrName, i + 1);
        fgets(numbersStr, 11, fp);
        numbers[i] = atoi(numbersStr);
    }
    
    printf("\n");
    
    return numbers;
}

int* getNumbers(int srcNum){
    if(SOURCES & 1 << (3 - srcNum)){
        return getFromFile(srcNum);
    }else{
        return getFromTerminal(srcNum);
    }
}

void sendNumbersToFile(int srcNum, int numbers[]){
    
    FILE *fp = fopen("file1.txt", "w+");
    
    int i;
    
    char number[11];
    for(i = 0; i < NUM_COUNT; i++) {
        sprintf(number, "%d", numbers[i]);
        fputs(number, fp);
    }
    
    fclose(fp);
}

#endif
