#include <stdlib.h>
#include <stdio.h>
#include "bintree.h"
#include "queue.h"


struct Queue* createQueue() {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->front = 0;
    queue->rear = 0;
    return queue;
}

int isQueueEmpty(struct Queue* queue) {
    return queue->front == queue->rear;
}

void enqueue(struct Queue* queue, struct TreeNode* node) {
    if ((queue->rear + 1) % MAX_QUEUE_SIZE == queue->front) {
        printf("Queue is full!\n");
        return;
    }
    queue->nodes[queue->rear] = node;
    queue->rear = (queue->rear + 1) % MAX_QUEUE_SIZE;
}

struct TreeNode* dequeue(struct Queue* queue) {
    if (isQueueEmpty(queue)) {
        printf("Queue is empty!\n");
        return NULL;
    }
    struct TreeNode* node = queue->nodes[queue->front];
    queue->front = (queue->front + 1) % MAX_QUEUE_SIZE;
    return node;
}
