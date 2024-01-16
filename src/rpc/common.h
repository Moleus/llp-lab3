#pragma once
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

Rpc__CreateFileNodeRequest *convert_to_rpc(CreateFileNodeRequest request);

Rpc__NodeId *convert_node_id_to_rpc(node_id_t node_id);

Rpc__FileInfo convert_file_info_to_rpc(FileInfo file_info);

CreateFileNodeRequest convert_from_rpc(Rpc__CreateFileNodeRequest request);

node_id_t convert_node_id_from_rpc(Rpc__NodeId *node_id);

