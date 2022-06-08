#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <stdbool.h>

#include "model.h"
#include "statistics.h"
#include "../util/priority_queue.h"
#include "../util/mt19937.h"

struct Engine {
    uint64_t num_iter, id;
    double time_now, time_end;
    struct MT19937 gen;
    double *rate_up;
    double *rate_down;
    struct Model model;
    struct Statistics statistics;
    struct PriorityQueue priority_queue;
};

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
);
int engine_finalize(struct Engine *engine);
int engine_run(struct Engine *engine, bool verbose, FILE *fout);

#endif
