#include <stdlib.h>
#include "util/priority_queue.h"

void print_func(void *data, FILE *fout) {
    fprintf(fout, "%lu\n", *(uint64_t*)data);
}

int main(void) {
    struct PriorityQueue pq;
    priority_queue_initialize(&pq, 5);
    uint64_t id[10];
    for (uint64_t i = 0, *ii = id; i < 10; ++i, ++ii) {
        *ii = i;
    }
    double timestamp[10] = {
        10,
        9,
        9,
        5,
        5,
        5,
        2,
        2,
        0.3,
        20
    };
    uint64_t *iter_id = id;
    double *iter_timestamp = timestamp;
    for (uint64_t i = 0; i < 10; ++i, ++iter_id, ++iter_timestamp) {
        uint64_t *temp = malloc(sizeof(uint64_t));
        *temp = i << 1;
        priority_queue_add(&pq, *iter_id, *iter_timestamp, temp);
    }
    priority_queue_log(&pq, print_func, stdout);
    for (uint64_t i = 0; i < 10; ++i, ++iter_id, ++iter_timestamp) {
        priority_queue_remove(&pq);
        priority_queue_log(&pq, print_func, stdout);
        putchar(10);
    }
    iter_id = id;
    iter_timestamp = timestamp;
    for (uint64_t i = 0; i < 10; ++i, ++iter_id, ++iter_timestamp) {
        uint64_t *temp = malloc(sizeof(uint64_t));
        *temp = i << 1;
        priority_queue_add(&pq, *iter_id, *iter_timestamp, temp);
    }
    priority_queue_log(&pq, print_func, stdout);
    for (uint64_t i = 0; i < 10; ++i, ++iter_id, ++iter_timestamp) {
        priority_queue_remove(&pq);
        priority_queue_log(&pq, print_func, stdout);
        putchar(10);
    }
    priority_queue_finalize(&pq);
    return 0;
}
