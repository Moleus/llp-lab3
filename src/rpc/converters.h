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

typedef struct {
    NodeValue value;
    node_id_t parent;
} CreateNodeRequest;

typedef struct {
    node_id_t node_id;
    node_id_t parent_id;
    NodeValue new_value;
} UpdateNodeRequest;

typedef struct {
    node_id_t node_id;
} DeleteNodeRequest;

Rpc__NodeId *convert_node_id_to_rpc(node_id_t node_id);

Rpc__FileInfo *convert_to_rpc_FileInfo(FileInfo file_info);

Rpc__CreateNodeRequest *convert_to_rpc_CreateNodeRequest(CreateNodeRequest request);

Rpc__UpdateNodeRequest *convert_to_rpc_UpdateNodeRequest(UpdateNodeRequest request);

Rpc__DeleteNodeRequest *convert_to_rpc_DeleteNodeRequest(DeleteNodeRequest request);

Rpc__NodeValue *convert_to_rpc_NodeValue(NodeValue value);

Rpc__Node *convert_to_rpc_Node(Node node);

node_id_t convert_from_rpc_nodeId(Rpc__NodeId *node_id);

CreateNodeRequest convert_from_rpc_CreateNodeRequest(Rpc__CreateNodeRequest request);

UpdateNodeRequest convert_from_rpc_UpdateNodeRequest(Rpc__UpdateNodeRequest request);

DeleteNodeRequest convert_from_rpc_DeleteNodeRequest(Rpc__DeleteNodeRequest request);

FileInfo convert_from_rpc_FileInfo(Rpc__FileInfo file_info);

NodeValue convert_from_rpc_NodeValue(Rpc__NodeValue value);
