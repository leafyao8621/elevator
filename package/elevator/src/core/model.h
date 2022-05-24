#ifndef _MODEL_H_
#define _MODEL_H_

#include <stdint.h>

struct Elevator {
    uint64_t floor;
    uint64_t weight;
};

struct Model {
    uint64_t num_elevators, num_floors, weight_limit;
    struct Elevator *elevators;
};

int model_initialize(
    struct Model *model,
    uint64_t num_elevators,
    uint64_t weight_limit
);

#endif