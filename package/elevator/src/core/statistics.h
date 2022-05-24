#ifndef _STATISTICS_H_
#define _STATISTICS_H_

#include <stdint.h>

struct Statistics {
    uint64_t *num_delivered;
    double *avg_tiq, *avg_tis;
};

#endif