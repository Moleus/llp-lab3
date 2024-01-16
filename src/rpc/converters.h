#pragma once

#include <stdbool.h>
#include "stdint.h"
#include "common.pb-c.h"
#include "public/structures.h"

typedef struct __attribute((packed)) {
    char* name;
    char* owner;
    uint64_t access_time;
    char* mime_type;
} FileInfo;

typedef struct CreateFileNodeRequest {
    node_id_t parent_id;
    FileInfo file_info;
} CreateFileNodeRequest;

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

typedef struct {
    NodeValue value;
    node_id_t parent;
} CreateNodeRequest;

Rpc__CreateFileNodeRequest *convert_to_rpc(CreateFileNodeRequest request);

Rpc__NodeId *convert_node_id_to_rpc(node_id_t node_id);

Rpc__FileInfo *convert_to_rpc_FileInfo(FileInfo file_info);

Rpc__CreateNodeRequest *convert_to_rpc_CreateNodeRequest(CreateNodeRequest request);

Rpc__NodeValue *convert_to_rpc_NodeValue(NodeValue value);


CreateFileNodeRequest convert_from_rpc_CreateFileNodeRequest(Rpc__CreateFileNodeRequest request);

node_id_t convert_from_rpc_nodeId(Rpc__NodeId *node_id);

CreateNodeRequest convert_from_rpc_CreateNodeRequest(Rpc__CreateNodeRequest request);

FileInfo convert_from_rpc_FileInfo(Rpc__FileInfo file_info);

NodeValue convert_from_rpc_NodeValue(Rpc__NodeValue value);
