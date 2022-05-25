#ifndef _MODEL_H_
#define _MODEL_H_

#include <stdint.h>
#include "../util/linked_list.h"

struct Agent {
    uint64_t destination;
    double time_entered;
};

struct Elevator {
    uint64_t floor;
    struct LinkedList payload;
};

struct Model {
    uint64_t num_elevators, num_floors, weight_limit;
    struct Elevator *elevators;
    struct LinkedList *queues;
};

int model_initialize(
    struct Model *model,
    uint64_t num_elevators,
    uint64_t num_floors,
    uint64_t weight_limit
);
int model_finalize(struct Model *model);
int model_log(struct Model *model, FILE *fout);

#endif