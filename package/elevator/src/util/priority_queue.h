#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_

#include <stdio.h>
#include <stdint.h>

struct PriorityQueue {
    uint64_t size, capacity;    
    struct PriorityQueueNode *data, *data_end;
};

int priority_queue_initialize(struct PriorityQueue *pq, uint64_t capacity);
int priority_queue_add(
    struct PriorityQueue *pq,
    uint64_t id,
    double timestamp,
    void *data
);
int priority_queue_remove(struct PriorityQueue *pq);
int priority_queue_finalize(struct PriorityQueue *pq);
int priority_queue_log(
    struct PriorityQueue *pq,
    void (*print_func)(void*, FILE*),
    FILE *fout
);

#endif
