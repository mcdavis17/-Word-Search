//
//  words.c
//  
//
//  Created by Michael Caleb Davis on 11/3/17.
//
//  This project takes a word search from a file and searches for a word which the user inputs.  All other letters are replaced with a period, leaving only the instances of the word showing.

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


void printArray(char **search, int size);
int LR(int size, char **search, char word[], char **emptySearch);
int RL(int size, char **search, char word[], char **emptySearch);
int UD(int size, char **search, char word[], char **emptySearch);
int DU(int size, char **search, char word[], char **emptySearch);

int main (int argc, char *argv[])
{
//    Opening File
    FILE *fp = fopen(argv[2], "r");
    if (fp == NULL) {
        printf("the file \"%s\" was not found\n", argv[2]);
        return -1;
    }
//    Allocating space
    int size = atoi(argv[1]);
    char **search = malloc(sizeof(char*) * (size + 1));
    if (search == NULL) {
        printf("there is not enough memory available\n");
        return -1;
    }
    for (int i=0; i<size; i++) {
        search[i] = malloc(sizeof(char) * size);
        if (search[i] == NULL) {
            printf("there is not enough memory available\n");
            return -1;
        }
    }
    char **emptySearch = malloc(sizeof(char*) * (size + 1));
    if (emptySearch == NULL) {
        printf("there is not enough memory available\n");
        return -1;
    }
    for (int i=0; i<size; i++) {
        emptySearch[i] = malloc(sizeof(char) * size);
        if (emptySearch[i] == NULL) {
            printf("there is not enough memory available\n");
            return -1;
        }
    }
//    Loading the empty array with '.'
    for (int r=0; r<size; r++) {
        for (int c=0; c<size; c++) {
            emptySearch[r][c] = '.';
        }
    }
    
    
//    Checking the number of arguments
    if (argc != 3) {
        printf("incorrect number of arguments in command line\n");
        return -1;
    }
    

//    Scanning in the word search
    for (int r=0; r<size; r++) {
        for (int c=0; c<size; c++) {
            fscanf(fp, " %c", &search[r][c]);
        }
    }
//    Printing the word search
    printf("The word puzzle is: \n");
    printArray(search, size);
    
//    Getting user input for the word to be searched for
    char word[size+1];
    printf("Enter a word to find in the puzzle: ");
    scanf("%s", word);
    
    int totalCount = 0;
//    Searching the word search for the word and then prompting for a new word
    while (strcmp(word, "zzz") != 0) {
        int LRcount = LR(size, search, word, emptySearch);
        int RLcount = RL(size, search, word, emptySearch);
        int UDcount = UD(size, search, word, emptySearch);
        int DUcount = DU(size, search, word, emptySearch);
        if (strlen(word) != 1) totalCount = LRcount + RLcount + DUcount + UDcount;
        else (totalCount = LRcount);
        printf("The word %s was found %d times as shown below\n", word, totalCount);
        printf("\t%d times left-to-right\n", LRcount);
        printf("\t%d times top-to-bottom\n", UDcount);
        printf("\t%d times bottom-to-top\n", DUcount);
        printf("\t%d times right-to-left\n", RLcount);
        printArray(emptySearch, size);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                emptySearch[i][j] = '.';
            }
        }
        printf("Enter another word to find (or 'zzz' to exit): ");
        scanf("%s", word);
    }
    
    fclose(fp);
    free(search);
    free(emptySearch);

    return 0;
}
//Function to reverse a word
void reverse(char word[]) {
    int max = strlen(word) - 1;
    int min = 0;
    char temp;
    while (min < max) {
        temp = word[min];
        word[min] = word[max];
        word[max] = temp;
        min++;
        max--;
    }
}
//Function to print the word search
void printArray(char **search, int size) {
    for (int r=0; r<size; r++) {
        printf("\t");
        for (int c=0; c<size; c++) {
            printf("%c", search[r][c]);
        }
        printf("\n");
    }
}
//Function to search left-to-right
int LR(int size, char **search, char word[], char **emptySearch) {
    int len = strlen(word);
    int count = 0;
    bool realWord = false;
    for (int r=0; r<size; r++) {
        for (int c=0; c<size-len+1; c++) {
            realWord = true;
            for (int l=0; l<len; l++) {
                if (word[l] != search[r][c+l]) {
                    realWord = false;
                    break;
                }
            }
            if (realWord == true) {
                for (int t=0; t<len; t++) emptySearch[r][c+t] = word[t];
                count++;
            }
        }
    }
    return count;
}
//Function to search right-to-left
int RL(int size, char **search, char word[], char **emptySearch) {
    char newWord[strlen(word)+1];
    int count = 0;
    strcpy(newWord, word);
    reverse(newWord);
    int len = strlen(word);
    bool isSearchedWord = false;
    for (int r=0; r<size; r++) {
        for (int c=0; c<size-len+1; c++) {
            isSearchedWord = true;
            for (int l=0; l<len; l++) {
                if (newWord[l] != search[r][c+l]) {
                    isSearchedWord = false;
                    break;
                }
            }
            if (isSearchedWord == true) {
                for (int t=0; t<strlen(newWord); t++) emptySearch[r][c+t] = newWord[t];
                count++;
            }
        }
    }
    return count;
}
//Function to search top-to-bottom
int UD(int size, char **search, char word[], char **emptySearch) {
    int len = strlen(word);
    int count = 0;
    bool realWord = false;
    for (int r=0; r<size-len+1; r++) {
        for (int c=0; c<size; c++) {
            realWord = true;
            for (int m=0; m<len; m++) {
                if (word[m] != search[r+m][c]) {
                    realWord = false;
                    break;
                }
            }
            if (realWord == true) {
                for (int t=0; t<strlen(word); t++) emptySearch[r+t][c] = word[t];
                count++;
            }
        }
    }
    return count;
}
//Function to search bottom-to-top
int DU(int size, char **search, char word[], char **emptySearch) {
    char newWord[strlen(word)+1];
    int count = 0;
    strcpy(newWord, word);
    reverse(newWord);
    int len = strlen(word);
    bool isSearchedWord = false;
    for (int r=0; r<size-len+1; r++) {
        for (int c=0; c<size; c++) {
            isSearchedWord = true;
            for (int p=0; p<len; p++) {
                if (newWord[p] != search[r+p][c]) {
                    isSearchedWord = false;
                    break;
                }
            }
            if (isSearchedWord == true) {
                for (int t=0; t<strlen(newWord); t++) emptySearch[r+t][c] = newWord[t];
                count++;
            }
        }
    }
    return count;
}



