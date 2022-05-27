#ifndef _ENGINE_H_
#define _ENGINE_H_

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
    double *rate_up,
    double *rate_down,
    double time_end,
    uint64_t num_iter
);

int engine_finalize(struct Engine *engine);

#endif