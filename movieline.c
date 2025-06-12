/*
Aryan Saraswat
COP3502
September 25, 2023
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a struct to represent a customer.
typedef struct Customer {
    char name[51];
    int tickets;
    int queueNum;
    int arrivalTime;
    int checkoutTime;
} Customer;

// Define a struct to represent a node in a linked list.
typedef struct Node {
    Customer *customer;
    struct Node *next;
} Node;

// Define a struct to represent a queue of customers.
typedef struct Queue {
    Node *front;
    Node *rear;
    int size;
} Queue;

// Function to create a new customer.
Customer *createCustomer(char name[], int tickets, int arrivalTime) {
    Customer *customer = (Customer*)malloc(sizeof(Customer));
    if (customer != NULL) {
        strcpy(customer->name, name);
        customer->tickets = tickets;
        customer->arrivalTime = arrivalTime;
        customer->checkoutTime = 0;
    }
    return customer;
}

// Function to create a new node for a customer.
Node *createNode(Customer *customer) {
    Node *node = (Node*)malloc(sizeof(Node));
    if (node != NULL) {
        node->customer = customer;
        node->next = NULL;
    }
    return node;
}

// Function to create a new queue.
Queue *createQueue() {
    Queue *queue = (Queue*)malloc(sizeof(Queue));
    if (queue != NULL) {
        queue->front = NULL;
        queue->rear = NULL;
        queue->size = 0;
    }
    return queue;
}

// Function to enqueue a customer into a queue.
void enqueue(Queue *queue, Customer *customer) {
    Node *node = createNode(customer);
    if (queue->rear == NULL) {
        queue->front = node;
        queue->rear = node;
    } else {
        queue->rear->next = node;
        queue->rear = node;
    }
    queue->size++;
}

// Function to dequeue a customer from a queue.
Customer *dequeue(Queue *queue) {
    if (queue->front == NULL) {
        return NULL;
    }
    Node *node = queue->front;
    Customer *customer = node->customer;
    queue->front = node->next;
    if (queue->front == NULL) {
        queue->rear = NULL; 
    }
    free(node);
    queue->size--;
    return customer;
}

// Function to peek at the front customer in a queue without dequeueing.
Customer *peek(Queue *queue) {
    if (queue->front == NULL) {
        return NULL;
    }
    return queue->front->customer;
}

// Function to check if a queue is empty.
int isEmpty(Queue *queue) {
    if (queue->size == 0) {
        return 1;
    }
    return 0;
}

// Function to get the size of a queue.
int getSize(Queue *queue) {
    return queue->size;
}

// Function to get the queue with the minimum size among an array of queues.
int getMinQueue(Queue *queues[]) {
    int max = 500000, x = 0, y;
    for (int i = 0; i < 12; i++) {
        if (queues[i]->size < max && queues[i]->size != 0) {
            y = i;
            max = queues[i]->size;
            x = 1;
        }
    }
    if (x == 1) {
        return y + 1;
    }
    return 0;
}

// Function to calculate the checkout time for a customer.
int calculateCheckoutTime(Customer *customer) {
    return (30 + customer->tickets * 5);
}

int main() {
    int n, b, k, x, y;
    scanf("%d %d", &n, &b);
    
    // Create an array of 12 queues.
    Queue *queues[12];
    for (int i = 0; i < 12; i++) {
        queues[i] = createQueue();
    }
    
    for(int i = 0; i < n; i++) {
        char name[51];
        int tickets, arrivalTime, p;
        scanf("%s %d %d", name, &tickets, &arrivalTime);
        
        // Create a new customer and determine their queue number.
        Customer *customer = createCustomer(name, tickets, arrivalTime);
        p = name[0] - 'A';
        if (p % 13 != 0) {
            customer->queueNum = p % 13;
        } else if (p % 13 == 0) {
            customer->queueNum = getMinQueue(queues);
        }
        // Enqueue the customer into the appropriate queue.
        enqueue(queues[customer->queueNum - 1], customer);
    }
    
    int boothsAssigned = 0;
    
    // Create an array to keep track of the number of customers assigned to each booth.
    int boothQueueCount[b];
    for (int i = 0; i < b; i++) {
        boothQueueCount[i] = 0;
    }

    for (int i = 0; i < 12; i++) {
        if (!isEmpty(queues[i])) {
            int boothNumber = boothsAssigned % b;
            boothsAssigned++;

            if (boothQueueCount[boothNumber] < (n / b)) {
                boothQueueCount[boothNumber]++;
            } else {
                boothNumber = boothsAssigned % b;
                boothsAssigned++;
            }

            Queue *currentQueue = queues[i];
            while (!isEmpty(currentQueue)) {
                Customer *customer = dequeue(currentQueue);
                customer->checkoutTime = customer->arrivalTime + calculateCheckoutTime(customer);
                printf("Booth %d\n", boothNumber + 1);
                printf("%s from line %d checks out at time %d.\n", customer->name, customer->queueNum, customer->checkoutTime);
                printf("\n");
            }
        }
    }

    // Free memory
    for (int i = 0; i < 12; i++) {
        while (!isEmpty(queues[i])) {
            Customer *customer = dequeue(queues[i]);
            free(customer);
        }
        free(queues[i]);
    }

    return 0;
}