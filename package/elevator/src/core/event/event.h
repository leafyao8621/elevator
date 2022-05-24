#ifndef _EVENT_H_
#define _EVENT_H_

#include <stdint.h>

struct Event {
    union {

    } data;
    int (*handler)(struct Event *event, struct Engine *engine);
};

#endif