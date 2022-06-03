#include "../../engine.h"
#include "../event.h"
#include "../../../util/err_code.h"

static int handler(struct Event *event, struct Engine *engine) {
    
    return 0;
}

int event_arrival_initialize(
    struct Event *event,
    uint8_t direction,
    uint64_t floor
) {
    if (!event) {
        return ERR_NULL_PTR;
    }
    event->data.arrival.direction = direction;
    event->data.arrival.floor = floor;
    event->handler = handler;
    return 0;
}
