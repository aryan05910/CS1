/*
Aryan Saraswat
COP3502C
September 12, 2023
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define INITSIZE 10
#define MAXLEN 50
#define MAXROWS 100000

//Struct that represents an order
typedef struct order {
    int s_seat;
    int e_seat;
    char* name;
} order;

//Struct that represents a theater row
typedef struct theaterrow {
    order** list_orders;
    int max_size;
    int cur_size;
} theaterrow;

//Creates a new order
order* make_order(int start, int end, char* this_name) {
    order* newOrder = (order*)malloc(sizeof(order));
    newOrder->s_seat = start;
    newOrder->e_seat = end;
    newOrder->name = (char*)malloc((strlen(this_name) + 1) * sizeof(char));
    strcpy(newOrder->name, this_name);
    return newOrder;
}

//Creates a new theater row
theaterrow* make_empty_row() {
    theaterrow* newRow = (theaterrow*)malloc(sizeof(theaterrow));
    newRow->list_orders = (order**)malloc(INITSIZE * sizeof(order*));
    newRow->max_size = INITSIZE;
    newRow->cur_size = 0;
    return newRow;
}

//Checks to see if two orders conflict with each other
int conflict(order* order1, order* order2) {
    if (order1->e_seat < order2->s_seat || order1->s_seat > order2->e_seat) {
        return 0;
    }
    return 1;
}

//Checks to see if an order can be added to a row
int can_add_order(theaterrow* this_row, order* this_order) {
    for (int i = 0; i < this_row->cur_size; i++) {
        if (conflict(this_row->list_orders[i], this_order)) {
            return 0;
        }
    }
    return 1;
}

//Adds an order to a row
void add_order(theaterrow* this_row, order* this_order) {
    if (this_row->cur_size == this_row->max_size) {
        this_row->max_size = this_row->max_size * 2;
        this_row->list_orders = (order**)realloc(this_row->list_orders, this_row->max_size * sizeof(order*));
    }
    this_row->list_orders[this_row->cur_size] = this_order;
    this_row->cur_size++;
}

//Checks if an order contains a given seat number
int contains(order* myorder, int seat_no) {
    if (seat_no >= myorder->s_seat && seat_no <= myorder->e_seat) {
        return 1;
    } else {
        return 0;
    }
}

//Gets the owner of a seat in a theater
char* get_owner(theaterrow** theater, int row, int seat_num) {
    if (theater[row] != NULL) {
        for (int i = 0; i < theater[row]->cur_size; i++) {
            if (contains(theater[row]->list_orders[i], seat_num) == 1) {
                return theater[row]->list_orders[i]->name;
            }
        }
    }
    return NULL;
}

//Gets the owner of a seat in a theater row
char* get_row_owner(theaterrow* this_row, int seat_num) {
    for (int i = 0; i < this_row->cur_size; i++) {
        if (contains(this_row->list_orders[i], seat_num)) {
            return this_row->list_orders[i]->name;
        }
    }
    return NULL;

}

//Frees the memory of an order
void free_order(order* this_order) {
    free(this_order->name);
    free(this_order);
}

//Frees the memory of a theater row
void free_row(theaterrow* this_row) {
    for (int i = 0; i < this_row->cur_size; i++) {
        free_order(this_row->list_orders[i]);
    }
    free(this_row->list_orders);
    free(this_row);
}

int main() {
    //Creates theater
    theaterrow** AMC = calloc(MAXROWS + 1, sizeof(theaterrow*));
    int x = 1;
    char input[10];
    
    //Keeps on asking for input until QUIT is inputted
    while (x == 1) {
        scanf("%s", input);
        
        //BUY input
        if (strcmp(input, "BUY") == 0) {
            int start, end, row;
            char name[MAXLEN];
            
            scanf("%d %d %d %s", &row, &start, &end, name);
            
            order* newOrder = make_order(start, end, name);
            
            if (AMC[row] == NULL) {
                AMC[row] = make_empty_row();
            }
            
            if (can_add_order(AMC[row], newOrder)) {
                add_order(AMC[row], newOrder);
                printf("SUCCESS\n");
            } else {
                printf("FAILURE\n");
                free_order(newOrder);
            }
        //LOOKUP input
        } else if (strcmp(input, "LOOKUP") == 0) {
            int seat, row;
            
            scanf("%d %d", &row, &seat);
            
            char* owner = get_owner(AMC, row, seat);
            if (owner == NULL) {
                printf("No one\n");
            } else {
                printf("%s\n", owner);
            }
        //QUIT input
        } else if ((strcmp(input, "QUIT")) == 0) {
            x = 0;
        }
    }
    //Frees memory
    for (int i = 0; i <= MAXROWS; i++) {
        free_row(AMC[i]);
    }
    free(AMC);
}
