#include <stdlib.h>
#include "core/model.h"

int main(void) {
    struct Model model;
    model_initialize(&model, 4, 10, 10);
    model_log(&model, stdout);
    model_finalize(&model);
    return 0;
}
