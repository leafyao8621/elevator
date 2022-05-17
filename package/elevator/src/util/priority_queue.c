#include <stdlib.h>

#include "priority_queue.h"
#include "err_code.h"

struct PriorityQueueNode {
    uint64_t id;
    double timestamp;
    void *data;
};

int priority_queue_initialize(struct PriorityQueue *pq, uint64_t capacity) {
    if (!pq) {
        return ERR_NULL_PTR;
    }
    pq->size = 0;
    pq->capacity = capacity;
    pq->data = malloc(sizeof(struct PriorityQueueNode) * capacity);
    if (!pq->data) {
        return ERR_OUT_OF_MEMORY;
    }
    pq->data_end = pq->data;
    return 0;
}

int priority_queue_finalize(struct PriorityQueue *pq) {
    if (!pq) {
        return ERR_NULL_PTR;
    }
    for (
        struct PriorityQueueNode *i = pq->data;
        i < pq->data_end;
        ++i
    ) {
        free(i->data);
    }
    free(pq->data);
    return 0;
}

int priority_queue_log(
    struct PriorityQueue *pq,
    void (*print_func)(void*, FILE*),
    FILE *fout
) {
    if (!pq) {
        return ERR_NULL_PTR;
    }
    fprintf(fout, "size: %lu\ncapacity: %lu\n", pq->size, pq->capacity);
    for (
        struct PriorityQueueNode *i = pq->data;
        i < pq->data_end;
        ++i
    ) {
        fprintf(fout, "id: %lu\ntimestamp: %lf\n", i->id, i->timestamp);
        print_func(i->data, fout);
    }
    return 0;
}

// static inline int comp(
//     struct PriorityQueueNode *a,
//     struct PriorityQueueNode *b
// ) {
//     if (a->timestamp == b->timestamp) {
//         return a->id - b->id;
//     }
//     if (a->timestamp > b->timestamp) {
//         return 1;
//     }
//     if (a->timestamp < b->timestamp) {
//         return -1;
//     }
//     return 0;
// }

int priority_queue_add(
    struct PriorityQueue *pq,
    uint64_t id,
    double timestamp,
    void *data
) {
    if (!pq) {
        return ERR_NULL_PTR;
    }
    if (pq->size == pq->capacity) {
        pq->capacity <<= 1;
        pq->data =
            realloc(pq->data, sizeof(struct PriorityQueueNode) * pq->capacity);
        if (!pq->data) {
            return ERR_OUT_OF_MEMORY;
        }
        pq->data_end = pq->data + pq->size;
    }
    ++pq->size;
    pq->data_end->id = id;
    pq->data_end->timestamp = timestamp;
    pq->data_end->data = data;
    ++pq->data_end;
    return 0;
}
