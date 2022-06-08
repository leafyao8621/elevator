#include <stdlib.h>
#include <math.h>

#include "../event.h"
#include "../../../util/err_code.h"

static int handler(struct Event *event, struct Engine *engine) {
    if (!event || !engine) {
        return ERR_NULL_PTR;
    }
    int ret =
        model_arrival(
            &engine->model,
            event->data.arrival.floor,
            event->data.arrival.destination,
            engine->time_now
        );
    if (ret) {
        return ret;
    }
    struct Event *next = 0;
    if (event->data.arrival.destination > event->data.arrival.floor) {
        uint64_t up_dest =
            mt19937_gen(&engine->gen) %
            (engine->model.num_floors - event->data.arrival.floor - 1) +
            event->data.arrival.floor + 1;
        next = malloc(sizeof(struct Event));
        if (!next) {
            return ERR_OUT_OF_MEMORY;
        }
        ret =
            event_arrival_initialize(next, event->data.arrival.floor, up_dest);
        if (ret) {
            return ret;
        }
        ret =
            priority_queue_add(
                &engine->priority_queue,
                engine->id++,
                engine->time_now -
                log(((double)mt19937_gen(&engine->gen)) / 0xffffffff) /
                engine->rate_up[event->data.arrival.floor],
                next
            );
        if (ret) {
            return ret;
        }
    } else {
        uint64_t down_dest =
            mt19937_gen(&engine->gen) % event->data.arrival.floor;
        next = malloc(sizeof(struct Event));
        if (!next) {
            return ERR_OUT_OF_MEMORY;
        }
        ret =
            event_arrival_initialize(
                next,
                event->data.arrival.floor,
                down_dest
            );
        if (ret) {
            return ret;
        }
        ret = 
            priority_queue_add(
                &engine->priority_queue,
                engine->id++,
                engine->time_now -
                log(((double)mt19937_gen(&engine->gen)) / 0xffffffff) /
                engine->rate_down[event->data.arrival.floor],
                next
            );
        if (ret) {
            return ret;
        }
    }
    return 0;
}

static int logger(struct Event *event, FILE *fout) {
    if (!event || !fout) {
        return ERR_NULL_PTR;
    }
    fprintf(
        fout,
        "event arrival:\nfloor: %lu\ndestination: %lu\n",
        event->data.arrival.floor,
        event->data.arrival.destination
    );
    return 0;
}

int event_arrival_initialize(
    struct Event *event,
    uint64_t floor,
    uint64_t destination
) {
    if (!event) {
        return ERR_NULL_PTR;
    }
    event->data.arrival.floor = floor;
    event->data.arrival.destination = destination;
    event->handler = handler;
    event->logger = logger;
    return 0;
}
