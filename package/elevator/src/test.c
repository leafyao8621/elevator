#include <stdlib.h>

#include "core/engine.h"
#include "util/err_code.h"

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
    engine_initialize(&engine, 10, 4, 10, rate_up, rate_down, 100, 10, 1000);
    int ret = engine_run(&engine, true, 0);
    if (ret) {
        printf("err code: %d\nerr str: %s\n", ret, err_str[ret - 1]);
    }
    engine_finalize(&engine);
    return 0;
}
