#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "engine.h"
#include "event/event.h"
#include "../util/err_code.h"

int engine_initialize(
    struct Engine *engine,
    uint64_t num_elevators,
    uint64_t num_floors,
    uint64_t weight_limit,
    double time_per_floor,
    double *rate_up,
    double *rate_down,
    double time_end,
    uint64_t num_iter,
    uint32_t seed
) {
    int ret =
        model_initialize(
            &engine->model,
            num_elevators,
            num_floors,
            weight_limit,
            time_per_floor
        );
    if (ret) {
        return ret;
    }
    ret = 
        priority_queue_initialize(&engine->priority_queue, 100);
    if (ret) {
        return ret;
    }
    engine->rate_up = malloc(sizeof(double) * num_floors);
    if (!engine->rate_up) {
        return ERR_OUT_OF_MEMORY;
    }
    memcpy(engine->rate_up, rate_up, sizeof(double) * num_floors);
    engine->rate_down = malloc(sizeof(double) * num_floors);
    if (!engine->rate_down) {
        return ERR_OUT_OF_MEMORY;
    }
    memcpy(engine->rate_down, rate_down, sizeof(double) * num_floors);
    engine->id = 0;
    engine->time_now = 0;
    engine->time_end = time_end;
    engine->num_iter = num_iter;
    if (!seed) {
        return ERR_INVALID_SEED;
    }
    mt19937_initialize(&engine->gen, seed);
    return 0;
}

int engine_finalize(struct Engine *engine) {
    if (!engine) {
        return ERR_NULL_PTR;
    }
    free(engine->rate_up);
    free(engine->rate_down);
    priority_queue_finalize(&engine->priority_queue);
    model_finalize(&engine->model);
    return 0;
}

static void event_logger(void *event, FILE *fout) {
    event_log((struct Event*)event, fout);
}

static void engine_log(struct Engine *engine, FILE *fout) {
    fprintf(fout, "time now: %lf\n", engine->time_now);
    fputs("event queue:\n", fout);
    priority_queue_log(&engine->priority_queue, event_logger, fout);
    model_log(&engine->model, fout);
}

int engine_run(struct Engine *engine, bool verbose, FILE *fout) {
    if (!engine) {
        return ERR_NULL_PTR;
    }
    if (verbose && fout == stdout) {
        return ERR_CONFLICTING_LOGGING;
    }
    int ret = 0;
    double *iter_rate_up = engine->rate_up;
    double *iter_rate_down = engine->rate_down;
    for (
        uint64_t i = 0;
        i < engine->model.num_floors;
        ++i, ++iter_rate_up, ++iter_rate_down
    ) {
        struct Event *up = 0;
        struct Event *down = 0;
        if (i != engine->model.num_floors - 1) {
            uint64_t up_dest =
                mt19937_gen(&engine->gen) %
                (engine->model.num_floors - i - 1) + i + 1;
            up = malloc(sizeof(struct Event));
            if (!up) {
                return ERR_OUT_OF_MEMORY;
            }
            ret = event_arrival_initialize(up, i, up_dest);
            if (ret) {
                return ret;
            }
        }
        if (i) {
            uint64_t down_dest = mt19937_gen(&engine->gen) % i;
            down = malloc(sizeof(struct Event));
            if (!down) {
                return ERR_OUT_OF_MEMORY;
            }
            ret = event_arrival_initialize(down, i, down_dest);
            if (ret) {
                return ret;
            }
        }
        if (up) {
            ret =
                priority_queue_add(
                    &engine->priority_queue,
                    engine->id++,
                    engine->time_now -
                    log(((double)mt19937_gen(&engine->gen)) / 0xffffffff) /
                    *iter_rate_up,
                    up
                );
            if (ret) {
                return ret;
            }
        }
        if (down) {
            ret = 
                priority_queue_add(
                    &engine->priority_queue,
                    engine->id++,
                    engine->time_now -
                    log(((double)mt19937_gen(&engine->gen)) / 0xffffffff) /
                    *iter_rate_down,
                    down
                );
            if (ret) {
                return ret;
            }
        }
    }
    if (verbose) {
        engine_log(engine, stdout);
    }
    if (fout) {
        engine_log(engine, fout);
    }
    for (; engine->time_now <= engine->time_end;) {
        if (verbose) {
            engine_log(engine, stdout);
        }
        if (fout) {
            engine_log(engine, fout);
        }
        ret = event_handle(engine->priority_queue.data->data, engine);
        if (ret) {
            return ret;
        }
    }
    return 0;
}
