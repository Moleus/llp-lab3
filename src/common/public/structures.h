#pragma once

#include <stdbool.h>
#include "stdint.h"
#include "public/structures.h"

typedef struct __attribute__((packed)) {
    const char *value;
    uint32_t length;
} String;

typedef struct __attribute__((packed)) {
    int32_t page_id;
    int32_t item_id;
} node_id_t;

typedef struct __attribute((packed)) {
    char* name;
    char* owner;
    uint64_t access_time;
    char* mime_type;
} FileInfo;

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
        FileInfo file_info_value;
    };
} NodeValue;
typedef struct __attribute__((packed)) {
    node_id_t id;
    node_id_t parent_id;
    NodeValue value;
} Node;