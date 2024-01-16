#pragma once
#include "stdint.h"

typedef struct __attribute__((packed)) {
    const char *value;
    uint32_t length;
} String;

typedef struct __attribute__((packed)) {
    int32_t page_id;
    int32_t item_id;
} node_id_t;
