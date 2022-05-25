#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "model.h"
#include "statistics.h"
#include "../util/priority_queue.h"
#include "../util/mt19937.h"

struct Engine {
    uint64_t num_iter, id;
    double time_now;
    struct MT19937 gen;
    double *rates;
    struct Model model;
    struct Statistics statistics;
    struct PriorityQueue priority_queue;
};

#endif