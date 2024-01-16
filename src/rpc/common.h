#pragma once
#include "stdint.h"
#include "common.pb-c.h"

typedef struct __attribute__((packed)) {
    const char *value;
    uint32_t length;
} String;

typedef struct __attribute((packed)) {
    char* name;
    char* owner;
    uint64_t access_time;
    char* mime_type;
} FileInfo;

typedef struct __attribute__((packed)) {
    int32_t page_id;
    int32_t item_id;
} node_id_t;

typedef struct CreateFileNodeRequest {
    node_id_t parent_id;
    FileInfo file_info;
} CreateFileNodeRequest;

CreateFileNodeRequest convert_from_rpc(Rpc__CreateFileNodeRequest request);

Rpc__CreateFileNodeRequest *convert_to_rpc(CreateFileNodeRequest request);

node_id_t convert_node_id_from_rpc(Rpc__NodeId *node_id);

Rpc__NodeId convert_node_id_to_rpc(node_id_t node_id);

Rpc__FileInfo convert_file_info_to_rpc(FileInfo file_info);
