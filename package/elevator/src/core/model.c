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
    model->queues = malloc(sizeof(struct LinkedList) * num_floors);
    if (!model->queues) {
        return ERR_OUT_OF_MEMORY;
    }
    struct LinkedList *iter_queue = model->queues;
    for (uint64_t i = 0; i < num_floors; ++i, ++iter_queue) {
        linked_list_initialize(iter_queue);
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
    struct LinkedList *iter_queue = model->queues;
    for (uint64_t i = 0; i < model->num_floors; ++i, ++iter_queue) {
        linked_list_finalize(iter_queue);
    }
    free(model->queues);
    return 0;
}

static void agent_logger(void *data, FILE *fout) {
    fprintf(
        fout,
        "destination: %lu\ntime_entered: %lf\n",
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
        fprintf(fout, "elevator %lu\n", i);
        linked_list_log(&iter_elevator->payload, agent_logger, fout);
    }
    struct LinkedList *iter_queue = model->queues;
    for (uint64_t i = 0; i < model->num_floors; ++i, ++iter_queue) {
        fprintf(fout, "floor %lu\n", i);
        linked_list_log(iter_queue, agent_logger, fout);
    }
    return 0;
}