#ifndef __QUEUE__
#define __QUEUE__

#define MAX_QUEUE_SIZE 100

struct Queue {
    struct TreeNode* nodes[MAX_QUEUE_SIZE];
    int front, rear;
};

int isQueueEmpty(struct Queue* queue);
struct Queue* createQueue();

void enqueue(struct Queue* queue, struct TreeNode* node);
struct TreeNode* dequeue(struct Queue* queue);

#endif