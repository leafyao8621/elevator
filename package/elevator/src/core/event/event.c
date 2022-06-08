#include "event.h"
#include "../../util/err_code.h"

int event_handle(struct Event *event, struct Engine *engine) {
    if (!event || !engine) {
        return ERR_NULL_PTR;
    }
    engine->time_now = engine->priority_queue.data->timestamp;
    int ret = event->handler(event, engine);
    if (ret) {
        return ret;
    }
    return priority_queue_remove(&engine->priority_queue);
}

int event_log(struct Event *event, FILE * fout) {
    if (!event || !fout) {
        return ERR_NULL_PTR;
    }
    return event->logger(event, fout);
}
