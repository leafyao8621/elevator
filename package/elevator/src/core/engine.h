#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "model.h"
#include "../util/priority_queue.h"

struct Engine {
    struct Model model;
    struct PriorityQueue priority_queue;
};

#endif