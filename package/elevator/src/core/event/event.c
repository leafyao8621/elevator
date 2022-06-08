#include "event.h"
#include "../../util/err_code.h"

int event_handle(struct Event *event, struct Engine *engine) {
    if (!event || !engine) {
        return ERR_NULL_PTR;
    }
    return event->handler(event, engine);
}

int event_log(struct Event *event, FILE * fout) {
    if (!event || !fout) {
        return ERR_NULL_PTR;
    }
    return event->logger(event, fout);
}
