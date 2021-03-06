#ifndef _MODEL_H_
#define _MODEL_H_

#include <stdint.h>
#include "../util/linked_list.h"

#define DIRECTION_UP 0
#define DIRECTION_DOWN 1

struct Agent {
    uint64_t destination;
    double time_entered;
};

struct Elevator {
    uint64_t floor;
    struct LinkedList payload;
};

struct Queue {
    struct LinkedList up;
    struct LinkedList down;
};

struct Model {
    uint64_t num_elevators, num_floors, weight_limit;
    double time_per_floor;
    struct Elevator *elevators;
    struct Queue *queues;
};

int model_initialize(
    struct Model *model,
    uint64_t num_elevators,
    uint64_t num_floors,
    uint64_t weight_limit,
    double time_per_floor
);
int model_finalize(struct Model *model);
int model_log(struct Model *model, FILE *fout);
int model_arrival(
    struct Model *model,
    uint64_t floor,
    uint64_t destination,
    double time_entered
);

#endif
