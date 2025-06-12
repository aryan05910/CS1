/*
Aryan Saraswat
August 24. 2023
COP3502
Assignment 0
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 100000
//Creates the frequency array for both strings
int *makeFreq(char *str){
    //Dynamically allocates memory of the array
    int *freq = calloc(26, sizeof(int));
    int len = strlen(str);
    //Counts how many of each letter is in the string and stores them at the proper index
    for(int i = 0; i<len; i++){
        if(str[i] >= 'A' && str[i] <= 'Z'){
            freq[str[i] - 'A']++;
        }
    }
    return freq;
}

int main()
{
    //Dynamically allocates the memory of the two strings and uses fgets to read the input into the strings
    char *str1 = (char*) malloc(sizeof(char) * (MAX+2));
    fgets(str1, MAX+1, stdin);
    char *str2 = (char*) malloc(sizeof(char) * (MAX+2));
    fgets(str2, MAX+1, stdin);
    
    //Makes the frequency array
    int *freq1 = makeFreq(str1);
    int *freq2 = makeFreq(str2);
    
    int sum = 0;
    //Compares the frequency of the letters from each string to see how many letters the new sign needs
    for(int i = 0; i<26; i++){
        if(freq2[i] > freq1[i]){
            //Adds up all the amount of letters needed for the new sign
            sum = sum + (freq2[i] - freq1[i]);
        }
    }
    //Prints out answer
    printf("%d", sum);
    free(str1);
    free(str2);
    free(freq1);
    free(freq2);
    
    
}



