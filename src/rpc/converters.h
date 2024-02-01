#pragma once

#include <stdbool.h>
#include "stdint.h"
#include "common.pb-c.h"
#include "public/structures.h"

typedef struct {
    node_id_t parent;
    NodeValue value;
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

Rpc__CreateNodeRequest *convert_to_rpc_CreateNodeRequest(CreateNodeRequest request);

Rpc__UpdateNodeRequest *convert_to_rpc_UpdateNodeRequest(UpdateNodeRequest request);

Rpc__DeleteNodeRequest *convert_to_rpc_DeleteNodeRequest(DeleteNodeRequest request);

Rpc__NodeValue *convert_to_rpc_NodeValue(NodeValue value);

Rpc__Nodes convert_to_rpc_Nodes(NodesArray *nodes);

Rpc__Node *convert_to_rpc_Node(Node node);

node_id_t convert_from_rpc_nodeId(Rpc__NodeId *node_id);

CreateNodeRequest convert_from_rpc_CreateNodeRequest(Rpc__CreateNodeRequest request);

UpdateNodeRequest convert_from_rpc_UpdateNodeRequest(Rpc__UpdateNodeRequest request);

DeleteNodeRequest convert_from_rpc_DeleteNodeRequest(Rpc__DeleteNodeRequest request);

NodeValue convert_from_rpc_NodeValue(Rpc__NodeValue value);

