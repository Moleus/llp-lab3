#pragma once

#include <stdbool.h>
#include "stdint.h"
#include "public/structures.h"

typedef struct __attribute__((packed)) {
    uint32_t length;
    char value[];
} String;

typedef struct __attribute__((packed)) {
    int32_t page_id;
    int32_t item_id;
} node_id_t;

typedef enum __attribute__((packed)) {
    INT_32,
    DOUBLE,
    STRING,
    BOOL,
    FILE_INFO
} ValueType;

typedef struct __attribute__((packed)) {
    ValueType type;
    union __attribute__((packed)) {
        int32_t int_value;
        double double_value;
        String string_value;
        bool bool_value;
    };
} NodeValue;
typedef struct __attribute__((packed)) {
    node_id_t id;
    node_id_t parent_id;
    NodeValue value;
} Node;

NodeValue *node_value_string_new(char *string);
Node *node_string_new(char *string);
