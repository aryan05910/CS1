/*
Aryan Saraswat
COP3502
P3-wheretositb.c
October 8, 2023
*/

#include <stdio.h>
#include <string.h>

#define MAX_N 10
#define MAX_NAME_LENGTH 20

int n, p;
int f = 0;
int firstOrder[MAX_N];
char names[MAX_N][MAX_NAME_LENGTH];
int popcorn[MAX_N];
int notNext[MAX_N][MAX_N];

//Checks to see if the permutation is valid
int isValidOrdering(int* perm) {
    int x, y;
    for (int i = 0; i < n; i++) {
        int current = perm[i];
        if(i > 0){
            x = perm[i - 1];
        }else{
            x = -1;
        }
        
        if(i < n - 1){
            y = perm[i + 1];
        }else{
            y = -1;
        }
        
        //Checks to see if the current person has popcorn or if neighbors have popcorn
        if (popcorn[current] == 0 && (x == -1 || popcorn[x] == 0) && (y == -1 || popcorn[y] == 0)) {
            return 0;
        }

        //Checks to see if the current person can't sit next to someone in the notNext array
        if (i < n - 1 && notNext[current][perm[i + 1]] == 1) {
            return 0; 
        }
    }
    return 1;
}

//Finds the first lexicographical ordering
void firstOrdering(int* perm, int* used, int k) {
    if (k == n && !f) {
        if (isValidOrdering(perm)) {
            f = 1;
            for (int i = 0; i < n; i++) {
                firstOrder[i] = perm[i];
            }
        }
        return;
    }
    for (int i = 0; i < n; i++) {
        if (!used[i] && !f) {
            used[i] = 1;
            perm[k] = i;
            firstOrdering(perm, used, k + 1);
            used[i] = 0;
        }
    }
}


int main() {
    //Reads in number of people and number of people with popcorn
    scanf("%d %d", &n, &p);
    
    //Reads in the names of people and who has popcorn or not
    for (int i = 0; i < n; i++) {
        scanf("%s %d", names[i], &popcorn[i]);
    }

    memset(notNext, 0, sizeof(notNext));

    for (int i = 0; i < p; i++) {
        char name1[MAX_NAME_LENGTH], name2[MAX_NAME_LENGTH];
        scanf("%s %s", name1, name2);
        int index1 = -1, index2 = -1;
        for (int j = 0; j < n; j++) {
            if (strcmp(names[j], name1) == 0) {
                index1 = j;
            }
            if (strcmp(names[j], name2) == 0) {
                index2 = j;
            }
        }
        if (index1 != -1 && index2 != -1) {
            notNext[index1][index2] = notNext[index2][index1] = 1;
        }
    }
    int perm[MAX_N];
    int used[MAX_N];
    firstOrdering(perm, used, 0);
    
    //Prints out the first lexicographical permutation
    for (int i = 0; i < n; i++) {
        printf("%s\n", names[firstOrder[i]]);
    }
    return 0;
}


