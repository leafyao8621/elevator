#ifndef _EVENT_H_
#define _EVENT_H_

#include <stdint.h>
#include <stdio.h>

struct Event {
    union {
        struct {
            uint8_t direction;
            uint64_t floor;
        } arrival;
    } data;
    int (*handler)(struct Event*, struct Engine*);
    int (*logger) (struct Event*, FILE*);
};

int event_handle(struct Event *event, struct Engine *engine);
int event_log(struct Event *event, FILE * fout);

int event_arrival_initialize(
    struct Event *event,
    uint8_t direction,
    uint64_t floor
);

#endif
