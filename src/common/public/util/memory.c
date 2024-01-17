#include <stdlib.h>
#include "memory.h"
#include "log.h"


void *my_alloc(size_t size) {
    void *p = calloc(size, 1);
    if (p == NULL) {
        LOG_ERR("Failed to allocate memory", "");
        exit(1);
    }
    return p;
}
