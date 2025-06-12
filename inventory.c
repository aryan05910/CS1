/*
Aryan Saraswat
COP3502
Program 6: inventory.c
December 3, 2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 19
#define TABLESIZE 300007

typedef struct item {
    char name[MAXLEN + 1];
    int quantity;
    int saleprice;
} item;

typedef struct node {
    item *iPtr;
    struct node *next;
} node;

typedef struct hashtable {
    node **lists;
    int size;
} hashtable;

//Hash function
int hashfunc(char *word, int size) {
    int len = strlen(word);
    int res = 0;
    for (int i = 0; i < len; i++)
        res = (1151 * res + (word[i] - 'a')) % size;
    return res;
}

//Creates a new item
item *createItem(char *name, int quantity, int saleprice) {
    item *Item = (item *)malloc(sizeof(item));
    strncpy(Item->name, name, MAXLEN);
    Item->quantity = quantity;
    Item->saleprice = saleprice;
    return Item;
}

//Creates a new node
node *createNode(item *iPtr) {
    node *Node = (node *)malloc(sizeof(node));
    Node->iPtr = iPtr;
    Node->next = NULL;
    return Node;
}

//Initialize hashtable
hashtable *intHT(int size) {
    hashtable *table = (hashtable *)malloc(sizeof(hashtable));
    table->lists = (node **)malloc(size * sizeof(node *));
    table->size = size;

    for (int i = 0; i < size; i++) {
        table->lists[i] = NULL;
    }

    return table;
}

//Inserts item into hashtable
void insertItem(hashtable *table, item *newItem) {
    int index = hashfunc(newItem->name, table->size);
    node *newNode = createNode(newItem);
    newNode->next = table->lists[index];
    table->lists[index] = newNode;
}

//Finds item in hashtable
item *findItem(hashtable *table, char *name) {
    int index = hashfunc(name, table->size);
    node *current = table->lists[index];

    while (current != NULL) {
        if (strcmp(current->iPtr->name, name) == 0) {
            return current->iPtr;
        }
        current = current->next;
    }

    return NULL;
}

//Buys supplies
void buySupplies(hashtable *table, char *name, int quantity, int totalprice, int *totalCash, int *totalComp) {
    item *Item = findItem(table, name);
    int listLength = 0;
    node *current = table->lists[hashfunc(name, table->size)];
    while (current != NULL) {
        listLength++;
        current = current->next;
    }
    if (Item != NULL) {
        Item->quantity += quantity;
        *totalCash -= totalprice;
        *totalComp += listLength;
        printf("%s %d %d\n", name, Item->quantity, *totalCash);
    } else {
        item *newItem = createItem(name, quantity, 0);
        insertItem(table, newItem);
        *totalCash -= totalprice;
        *totalComp += listLength + 1;
        printf("%s %d %d\n", name, newItem->quantity, *totalCash);
    }
}


//Sells items
void sellItems(hashtable *table, char *name, int quantity, int *totalCash, int *totalComp) {
    item *Item = findItem(table, name);

    if (Item != NULL) {
        int soldQuantity = (quantity < Item->quantity) ? quantity : Item->quantity;
        Item->quantity -= soldQuantity;
        *totalCash += soldQuantity * Item->saleprice;
        *totalComp += 1;
        printf("%s %d %d\n", name, Item->quantity, *totalCash);
    }
}

//Changes item price
void changePrice(hashtable *table, char *name, int newPrice) {
    item *Item = findItem(table, name);

    if (Item != NULL) {
        Item->saleprice = newPrice;
    }
}

int main() {
    int n;
    scanf("%d", &n);

    hashtable *inv = intHT(TABLESIZE);
    int totalCash = 100000;
    int totalComp = 0;

    for (int i = 0; i < n; i++) {
        char command[20];
        scanf("%s", command);
        //Commands
        if (strcmp(command, "buy") == 0) {
            char name[MAXLEN + 1];
            int quantity, totalprice;
            scanf("%s %d %d", name, &quantity, &totalprice);
            buySupplies(inv, name, quantity, totalprice, &totalCash, &totalComp);
        } else if (strcmp(command, "sell") == 0) {
            char name[MAXLEN + 1];
            int quantity;
            scanf("%s %d", name, &quantity);
            sellItems(inv, name, quantity, &totalCash, &totalComp);
        } else if (strcmp(command, "change_price") == 0) {
            char name[MAXLEN + 1];
            int newPrice;
            scanf("%s %d", name, &newPrice);
            changePrice(inv, name, newPrice);
        }
    }

    printf("%d\n%d\n", totalCash, totalComp);

    //Frees allocated memory
    for (int i = 0; i < TABLESIZE; i++) {
        node *current = inv->lists[i];
        while (current != NULL) {
            node *temp = current;
            current = current->next;
            free(temp->iPtr);
            free(temp);
        }
    }
    free(inv->lists);
    free(inv);

    return 0;
}

