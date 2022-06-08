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
    return ret;
}

static int logger(struct Event *event, FILE *fout) {
    if (!event || !fout) {
        return ERR_NULL_PTR;
    }
    fprintf(
        fout,
        "event arrival\nfloor: %lu\ndestination: %lu\n",
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
