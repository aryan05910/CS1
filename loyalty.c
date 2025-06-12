/*
Aryan Saraswat
COP3502
Program 5: loyalty.c
November 14, 2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 19

typedef struct customer {
    char name[MAXLEN + 1];
    int points;
} customer;

typedef struct treenode {
    customer* cPtr;
    int size; 
    struct treenode* left;
    struct treenode* right;
} treenode;

//Compare function for binary search tree
int compTreeNodes(const void* a, const void* b) {
    return strcmp(((treenode*)a)->cPtr->name, ((treenode*)b)->cPtr->name);
}

//Compare function for sorting customer data
int compCustomers(const void* a, const void* b) {
    const customer* customerA = *(const customer**)a;
    const customer* customerB = *(const customer**)b;

    //Sort by descending points
    if (customerA->points != customerB->points) {
        return customerB->points - customerA->points;
    }

    //Break ties alphabetically
    return strcmp(customerA->name, customerB->name);
}

//Creates a new treenode
treenode* create(customer* cPtr) {
    treenode* newNode = (treenode*)malloc(sizeof(treenode));
    newNode->cPtr = cPtr;
    newNode->size = 1;
    newNode->left = newNode->right = NULL;
    return newNode;
}

//Inserts a customer into the binary search tree
treenode* insert(treenode* root, customer* cPtr) {
    if (root == NULL) {
        return create(cPtr);
    }

    int cmp = strcmp(cPtr->name, root->cPtr->name);

    if (cmp < 0) {
        root->left = insert(root->left, cPtr);
    } else if (cmp > 0) {
        root->right = insert(root->right, cPtr);
    } else {
        root->cPtr->points += cPtr->points;
    }
    
    //Updates the size of the subtree rooted at this node
    root->size = 1 + (root->left ? root->left->size : 0) + (root->right ? root->right->size : 0);

    return root;
}

//Searchs for a customer in the binary search tree
treenode* search(treenode* root, char* name) {
    if (root == NULL || strcmp(name, root->cPtr->name) == 0) {
        return root;
    }

    if (strcmp(name, root->cPtr->name) < 0) {
        return search(root->left, name);
    }

    return search(root->right, name);
}

//Deletes a node from the binary search tree
treenode* delete(treenode* root, char* name) {
    if (root == NULL) {
        return root;
    }

    int cmp = strcmp(name, root->cPtr->name);

    if (cmp < 0) {
        root->left = delete(root->left, name);
    } else if (cmp > 0) {
        root->right = delete(root->right, name);
    } else {
        if (root->left == NULL) {
            treenode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            treenode* temp = root->left;
            free(root);
            return temp;
        }

        treenode* temp = root->right;
        while (temp->left != NULL) {
            temp = temp->left;
        }

        root->cPtr = temp->cPtr;

        root->right = delete(root->right, temp->cPtr->name);
    }

    root->size = 1 + (root->left ? root->left->size : 0) + (root->right ? root->right->size : 0);

    return root;
}

//Counts the number of names in the binary search tree that come before a particular customer
int countSmaller(treenode* root, char* name) {
    if (root == NULL) {
        return 0;
    }

    if (strcmp(name, root->cPtr->name) <= 0) {
        return countSmaller(root->left, name);
    }

    return 1 + (root->left ? root->left->size : 0) + countSmaller(root->right, name);
}

//Swaps two customer pointers
void swap(customer** a, customer** b) {
    customer* temp = *a;
    *a = *b;
    *b = temp;
}

//Partition function for Quick Sort
int partition(customer** customers, int low, int high) {
    customer* pivot = customers[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (compCustomers(customers[j], pivot) > 0) {
            i++;
            swap(&customers[i], &customers[j]);
        }
    }

    swap(&customers[i + 1], &customers[high]);
    return i + 1;
}

//Perform Quick Sort on an array of customer pointers
void quickSort(customer** customers, int low, int high) {
    if (low < high) {
        int pi = partition(customers, low, high);
        quickSort(customers, low, pi - 1);
        quickSort(customers, pi + 1, high);
    }
}

int main() {
    int n;
    scanf("%d", &n);

    treenode* root = NULL;

    //Process commands
    for (int i = 0; i < n; i++) {
        char command[15];
        scanf("%s", command);

        if (strcmp(command, "add") == 0) {
            char name[MAXLEN + 1];
            int points;
            scanf("%s %d", name, &points);

            customer* newCustomer = (customer*)malloc(sizeof(customer));
            strcpy(newCustomer->name, name);
            newCustomer->points = points;

            root = insert(root, newCustomer);

            printf("%s %d\n", name, newCustomer->points);
        } else if (strcmp(command, "sub") == 0) {
            char name[MAXLEN + 1];
            int points;
            scanf("%s %d", name, &points);

            treenode* result = search(root, name);

            if (result == NULL) {
                printf("%s not found\n", name);
            } else {
                result->cPtr->points = (result->cPtr->points > points) ? (result->cPtr->points - points) : 0;
                printf("%s %d\n", name, result->cPtr->points);
            }
        } else if (strcmp(command, "del") == 0) {
            char name[MAXLEN + 1];
            scanf("%s", name);

            treenode* result = search(root, name);

            if (result == NULL) {
                printf("%s not found\n", name);
            } else {
                root = delete(root, name);
                printf("%s deleted\n", name);
            }
        } else if (strcmp(command, "search") == 0) {
            char name[MAXLEN + 1];
            scanf("%s", name);

            treenode* result = search(root, name);

            if (result == NULL) {
                printf("%s not found\n", name);
            } else {
                printf("%s %d %d\n", result->cPtr->name, result->cPtr->points, countSmaller(root, name));
            }
        } else if (strcmp(command, "count_smaller") == 0) {
            char name[MAXLEN + 1];
            scanf("%s", name);

            printf("%d\n", countSmaller(root, name));
        }
    }

    //Creates an array to store pointers to each struct storing customer data
    customer** customerArray = (customer**)malloc(root->size * sizeof(customer*));

    // Traverse the tree and populate the array
    int index = 0;
    void populateArray(treenode* node) {
        if (node != NULL) {
            customerArray[index++] = node->cPtr;
            populateArray(node->left);
            populateArray(node->right);
        }
    }

    populateArray(root);

    //Sorts the array using Quick Sort
    quickSort(customerArray, 0, root->size - 1);

    //Print the sorted array
    for (int i = 0; i < root->size; i++) {
        printf("%s %d\n", customerArray[i]->name, customerArray[i]->points);
    }

    //Free allocated memory
    free(customerArray);
    free(root);

    return 0;
}
