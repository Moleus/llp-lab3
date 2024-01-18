#include "converters.h"
#include <malloc.h>
#include "common.pb-c.h"
#include "public/util/common.h"
#include "public/structures.h"
#include "public/util/memory.h"


node_id_t convert_from_rpc_nodeId(Rpc__NodeId *node_id) {
    ASSERT_ARG_NOT_NULL(node_id)
    node_id_t result;
    result.page_id = (int32_t) node_id->page_id;
    result.item_id = (int32_t) node_id->item_id;
    return result;
}

Rpc__NodeId *convert_node_id_to_rpc(node_id_t node_id) {
    Rpc__NodeId *result = malloc(sizeof(Rpc__NodeId));
    Rpc__NodeId tmp_result = RPC__NODE_ID__INIT;
    tmp_result.page_id = (uint32_t) node_id.page_id;
    tmp_result.item_id = (uint32_t) node_id.item_id;
    *result = tmp_result;
    return result;
}

Rpc__CreateNodeRequest *convert_to_rpc_CreateNodeRequest(CreateNodeRequest request) {
    Rpc__NodeId *parent_id = convert_node_id_to_rpc(request.parent);

    Rpc__CreateNodeRequest *createNodeRequest = malloc(sizeof(Rpc__CreateNodeRequest));
    Rpc__CreateNodeRequest tmp_createNodeRequest = RPC__CREATE_NODE_REQUEST__INIT;
    tmp_createNodeRequest.parent_id = parent_id;
    tmp_createNodeRequest.value = convert_to_rpc_NodeValue(request.value);
    *createNodeRequest = tmp_createNodeRequest;

    return createNodeRequest;
}

Rpc__UpdateNodeRequest *convert_to_rpc_UpdateNodeRequest(UpdateNodeRequest request) {
    Rpc__NodeId *node_id = convert_node_id_to_rpc(request.node_id);

    Rpc__UpdateNodeRequest *updateNodeRequest = malloc(sizeof(Rpc__UpdateNodeRequest));
    Rpc__UpdateNodeRequest tmp_updateNodeRequest = RPC__UPDATE_NODE_REQUEST__INIT;
    tmp_updateNodeRequest.id = node_id;
    tmp_updateNodeRequest.value = convert_to_rpc_NodeValue(request.new_value);
    *updateNodeRequest = tmp_updateNodeRequest;

    return updateNodeRequest;
}

Rpc__DeleteNodeRequest *convert_to_rpc_DeleteNodeRequest(DeleteNodeRequest request) {
    Rpc__NodeId *node_id = convert_node_id_to_rpc(request.node_id);

    Rpc__DeleteNodeRequest *deleteNodeRequest = malloc(sizeof(Rpc__DeleteNodeRequest));
    Rpc__DeleteNodeRequest tmp_deleteNodeRequest = RPC__DELETE_NODE_REQUEST__INIT;
    tmp_deleteNodeRequest.id = node_id;
    *deleteNodeRequest = tmp_deleteNodeRequest;

    return deleteNodeRequest;
}

Rpc__NodeValue *convert_to_rpc_NodeValue(NodeValue value) {
    Rpc__NodeValue *result = malloc(sizeof(Rpc__NodeValue));
    Rpc__NodeValue tmp_result = RPC__NODE_VALUE__INIT;

    tmp_result.type = (Rpc__NodeValue__Type) value.type;
    switch (value.type) {
        case INT_32:
            tmp_result.int_value = value.int_value;
            tmp_result.value_case = RPC__NODE_VALUE__VALUE_INT_VALUE;
            break;
        case DOUBLE:
            tmp_result.double_value = value.double_value;
            tmp_result.value_case = RPC__NODE_VALUE__VALUE_DOUBLE_VALUE;
            break;
        case STRING:
            tmp_result.string_value = my_alloc(sizeof(char*) * (value.string_value.length + 1));
            strcpy(tmp_result.string_value, value.string_value.value);
            tmp_result.value_case = RPC__NODE_VALUE__VALUE_STRING_VALUE;
            break;
        case BOOL:
            tmp_result.bool_value = value.bool_value;
            tmp_result.value_case = RPC__NODE_VALUE__VALUE_BOOL_VALUE;
            break;
        default:
            LOG_ERR("Unknown type: %d", value.type);
            exit(1);
    }
    *result = tmp_result;
    return result;
}

Rpc__Node *convert_to_rpc_Node(Node node) {
    Rpc__Node *result = malloc(sizeof(Rpc__Node));
    Rpc__Node tmp_result = RPC__NODE__INIT;
    tmp_result.id = convert_node_id_to_rpc(node.id);
    tmp_result.parent_id = convert_node_id_to_rpc(node.parent_id);
    tmp_result.value = convert_to_rpc_NodeValue(node.value);
    *result = tmp_result;
    return result;
}

CreateNodeRequest convert_from_rpc_CreateNodeRequest(Rpc__CreateNodeRequest request) {
    CreateNodeRequest result = {
            .parent = convert_from_rpc_nodeId(request.parent_id),
            .value = convert_from_rpc_NodeValue(*request.value)
    };
    return result;
}

UpdateNodeRequest convert_from_rpc_UpdateNodeRequest(Rpc__UpdateNodeRequest request) {
    UpdateNodeRequest result = {
            .node_id = convert_from_rpc_nodeId(request.id),
            .new_value = convert_from_rpc_NodeValue(*request.value)
    };
    return result;
}

DeleteNodeRequest convert_from_rpc_DeleteNodeRequest(Rpc__DeleteNodeRequest request) {
    DeleteNodeRequest result = {
            .node_id = convert_from_rpc_nodeId(request.id)
    };
    return result;
}

NodeValue convert_from_rpc_NodeValue(Rpc__NodeValue value) {
    NodeValue result = {
            .type = (ValueType) value.type
    };
    switch (value.type) {
        case INT_32:
            result.int_value = value.int_value;
            break;
        case DOUBLE:
            result.double_value = value.double_value;
            break;
        case STRING:
            result = node_value_string_new(value.string_value);
            break;
        case BOOL:
            result.bool_value = value.bool_value;
            break;
        default:
            LOG_ERR("Unknown type: %d", value.type);
            exit(1);
    }
    return result;
}