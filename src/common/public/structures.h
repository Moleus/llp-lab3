#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include "stdint.h"
#include "public/structures.h"

typedef struct __attribute__((packed)) {
    uint32_t length;
    char value[256];
} String;

typedef struct __attribute__((packed)) {
    int32_t page_id;
    int32_t item_id;
} node_id_t;

typedef enum __attribute__((packed)) {
    UNKNOWN,
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

typedef struct {
    size_t count;
    Node nodes[];
} NodesArray;

NodeValue node_value_string_new(char *string);
Node *node_string_new(char *string);

typedef struct {
    char* name;
    char* owner;
    char* access_time;
    char* mime_type;
} FileInfo;

FileInfo parse_file_info(char* file_info);

typedef enum {
    FILE_INFO_NAME = 1,
    FILE_INFO_OWNER = 2,
    FILE_INFO_ACCESS_TIME = 4,
    FILE_INFO_MIME_TYPE = 8
} FileInfoAttributes;

FileInfoAttributes file_info_attribute_from_string(char* attribute);

bool file_info_attribute_matches(FileInfoAttributes attr, char* value, FileInfo file_info);