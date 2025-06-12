/*
Aryan Saraswat
October 29, 2023
COP3502
Assignment P4: projector.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Creates a struct to store group info
typedef struct {
    double angle;  
    int s;      
} group;

//Sorts groups based on their angles
int compare(const void *a, const void *b) {
    return ((group*)a)->angle- ((group*)b)->angle;
}

//Finds the least amount of people that need to be covered by angle a
int minPeople(group *groups, int n, int a) {
    int current = 0;
    int min = 1000;
    int start = 0;
    int end = 0;
    while (end < 2*n) {
        double c = groups[end].angle - groups[start].angle;
        if (c < a) {
            current += groups[end].s;
            end++;
        } else {
            current -= groups[start].s;
            start++;
        }
        if (current < min) {
            min = current;
        }
    }
    return min;
}

//Calculates the maximum angle without anyone in the projection
double calcMaxAngle(group* groups, int n) {
    double max = 0.0;
    for (int i = 1; i < 2 * n; i++) {
        if (groups[i].angle - groups[i - 1].angle > max) {
            max = groups[i].angle - groups[i - 1].angle;
        }
    }
    return max;
}

int main() {
    int n, a;
    scanf("%d %d", &n, &a);

    group* groups = (group*)malloc(2* n* sizeof(group));

    for (int i = 0; i < n; i++) {
        double x, y;
        int s;
        scanf("%lf %lf %d", &x, &y, &s);
        double angle = fmod(atan2(y, x)* 180.0/ M_PI+ 360, 360);
        groups[i*2].angle = angle;
        groups[i*2].s = s;
        groups[i*2+ 1].angle = angle+ 360;
        groups[i*2+ 1].s = s;
    }

    qsort(groups, 2*n, sizeof(group), compare);

    int min = minPeople(groups, n, a);
    double max = calcMaxAngle(groups, n);

    printf("%d\n%.4lf\n", min, max);
    free(groups);

    return 0;
}
