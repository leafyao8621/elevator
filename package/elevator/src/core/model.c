#include <stdlib.h>

#include "model.h"
#include "../util/err_code.h"

int model_initialize(
    struct Model *model,
    uint64_t num_elevators,
    uint64_t num_floors,
    uint64_t weight_limit
) {
    if (!model) {
        return ERR_NULL_PTR;
    }
    model->num_elevators = num_elevators;
    model->num_floors = num_floors;
    model->weight_limit = weight_limit;
    model->elevators = malloc(sizeof(struct Elevator) * num_elevators);
    if (!model->elevators) {
        return ERR_OUT_OF_MEMORY;
    }
    struct Elevator *iter_elevator = model->elevators;
    for (uint64_t i = 0; i < num_elevators; ++i, ++iter_elevator) {
        linked_list_initialize(&iter_elevator->payload);
    }
    model->queues = malloc(sizeof(struct Queue) * num_floors);
    if (!model->queues) {
        return ERR_OUT_OF_MEMORY;
    }
    struct Queue *iter_queue = model->queues;
    for (uint64_t i = 0; i < num_floors; ++i, ++iter_queue) {
        linked_list_initialize(&iter_queue->up);
        linked_list_initialize(&iter_queue->down);
    }
    return 0;
}

int model_finalize(struct Model *model) {
    if (!model) {
        return ERR_NULL_PTR;
    }
    struct Elevator *iter_elevator = model->elevators;
    for (uint64_t i = 0; i < model->num_elevators; ++i, ++iter_elevator) {
        linked_list_finalize(&iter_elevator->payload);
    }
    free(model->elevators);
    struct Queue *iter_queue = model->queues;
    for (uint64_t i = 0; i < model->num_floors; ++i, ++iter_queue) {
        linked_list_finalize(&iter_queue->up);
        linked_list_finalize(&iter_queue->down);
    }
    free(model->queues);
    return 0;
}

static void agent_logger(void *data, FILE *fout) {
    fprintf(
        fout,
        "destination: %lu\ntime entered: %lf\n",
        ((struct Agent*)data)->destination,
        ((struct Agent*)data)->time_entered
    );
}

int model_log(struct Model *model, FILE *fout) {
    if (!model || !fout) {
        return ERR_NULL_PTR;
    }
    fprintf(fout, "weight limit: %lu\n", model->weight_limit);
    struct Elevator *iter_elevator = model->elevators;
    for (uint64_t i = 0; i < model->num_elevators; ++i, ++iter_elevator) {
        fprintf(fout, "elevator %lu:\n", i);
        linked_list_log(&iter_elevator->payload, agent_logger, fout);
    }
    struct Queue *iter_queue = model->queues;
    for (uint64_t i = 0; i < model->num_floors; ++i, ++iter_queue) {
        fprintf(fout, "floor %lu:\n", i);
        fputs("up:\n", fout);
        linked_list_log(&iter_queue->up, agent_logger, fout);
        fputs("down:\n", fout);
        linked_list_log(&iter_queue->down, agent_logger, fout);
    }
    return 0;
}

int model_arrival(
    struct Model *model,
    uint64_t floor,
    uint64_t destination,
    double time_entered
) {
    if (!model) {
        return ERR_NULL_PTR;
    }
    if (floor > model->num_floors) {
        return ERR_IDX_OUT_OF_BOUND;
    }
    struct Agent *temp = malloc(sizeof(struct Agent));
    temp->destination = destination;
    temp->time_entered = time_entered;
    int ret =
        linked_list_push_back(
            destination < floor ?
            &model->queues[floor].down :
            &model->queues[floor].up,
            temp
        );
    if (ret) {
        return ret;
    }
    return 0;
}
