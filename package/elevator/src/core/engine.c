#include <stdlib.h>
#include <string.h>
#include "engine.h"
#include "../util/err_code.h"

int engine_initialize(
    struct Engine *engine,
    uint64_t num_elevators,
    uint64_t num_floors,
    uint64_t weight_limit,
    double *rate_up,
    double *rate_down,
    double time_end,
    uint64_t num_iter,
    uint32_t seed
) {
    int ret =
        model_initialize(
            &engine->model,
            num_elevators,
            num_floors,
            weight_limit
        );
    if (ret) {
        return ret;
    }
    ret = 
        priority_queue_initialize(&engine->priority_queue, 100);
    if (ret) {
        return ret;
    }
    engine->rate_up = malloc(sizeof(double) * num_floors);
    if (!engine->rate_up) {
        return ERR_OUT_OF_MEMORY;
    }
    memcpy(engine->rate_up, rate_up, sizeof(double) * num_floors);
    engine->rate_down = malloc(sizeof(double) * num_floors);
    if (!engine->rate_down) {
        return ERR_OUT_OF_MEMORY;
    }
    memcpy(engine->rate_down, rate_down, sizeof(double) * num_floors);
    engine->time_now = 0;
    engine->time_end = time_end;
    engine->num_iter = num_iter;
    if (!seed) {
        return ERR_INVALID_SEED;
    }
    mt19937_initialize(&engine->gen, seed);
    return 0;
}

int engine_finalize(struct Engine *engine) {
    if (!engine) {
        return ERR_NULL_PTR;
    }
    free(engine->rate_up);
    free(engine->rate_down);
    priority_queue_finalize(&engine->priority_queue);
    model_finalize(&engine->model);
    return 0;
}