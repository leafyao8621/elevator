#include <stdlib.h>

#include "priority_queue.h"
#include "err_code.h"

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

static inline int comp(
    struct PriorityQueueNode *a,
    struct PriorityQueueNode *b
) {
    if (a->timestamp == b->timestamp) {
        return a->id - b->id;
    }
    if (a->timestamp > b->timestamp) {
        return 1;
    }
    if (a->timestamp < b->timestamp) {
        return -1;
    }
    return 0;
}

static inline void swap(
    struct PriorityQueueNode *a,
    struct PriorityQueueNode *b
) {
    static struct PriorityQueueNode temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

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
    pq->data_end->id = id;
    pq->data_end->timestamp = timestamp;
    pq->data_end->data = data;
    ++pq->data_end;
    for (uint64_t idx = ++pq->size; idx > 1; idx >>= 1) {
        if (comp(&pq->data[idx - 1], &pq->data[(idx >> 1) - 1]) > 0) {
            break;
        }
        swap(pq->data + idx - 1, pq->data + (idx >> 1) - 1);
    }
    return 0;
}

int priority_queue_remove(struct PriorityQueue *pq) {
    if (!pq) {
        return ERR_NULL_PTR;
    }
    if (!pq->size) {
        return ERR_PRIORITY_QUEUE_EMPTY;
    }
    --pq->size;
    --pq->data_end;
    free(pq->data->data);
    swap(pq->data, pq->data_end);
    for (uint64_t idx = 0; idx < pq->size;) {
        uint64_t left_idx = (idx << 1) + 1;
        uint64_t right_idx = (idx << 1) + 2;
        uint64_t tgt_idx = 0;
        if (right_idx < pq->size) {
            if (comp(pq->data + left_idx, pq->data + right_idx) < 0) {
                tgt_idx = left_idx;
            } else {
                tgt_idx = right_idx;
            }
            
        } else if (left_idx < pq->size) {
            tgt_idx = left_idx;
        } else {
            break;
        }
        if (comp(pq->data + idx, pq->data + tgt_idx) > 0) {
            swap(pq->data + idx, pq->data + tgt_idx);
            idx = tgt_idx;
        } else {
            break;
        }
    }
    return 0;
}
