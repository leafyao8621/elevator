#include <stdlib.h>
#include "core/engine.h"

int main(void) {
    struct Engine engine;
    double rate_up[4] = {
        1,
        2,
        3,
        4
    };
    double rate_down[4] = {
        1,
        2,
        3,
        4
    };
    engine_initialize(&engine, 4, 10, 10, rate_up, rate_down, 10000, 10, 1000);
    engine_finalize(&engine);
    return 0;
}
