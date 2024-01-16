#include "converters.h"
#include <malloc.h>
#include "common.pb-c.h"
#include "public/util/common.h"


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

Rpc__NodeValue *convert_to_rpc_NodeValue(NodeValue value) {
    Rpc__NodeValue *result = malloc(sizeof(Rpc__NodeValue));
    Rpc__NodeValue tmp_result = RPC__NODE_VALUE__INIT;

    tmp_result.type = (Rpc__NodeValue__Type) value.type;
    switch (value.type) {
        case INT_32:
            tmp_result.int_value = value.int_value;
            break;
        case DOUBLE:
            tmp_result.double_value = value.double_value;
            break;
        case STRING:
            tmp_result.string_value = strdup(value.string_value.value);
            break;
        case BOOL:
            tmp_result.bool_value = value.bool_value;
            break;
        case FILE_INFO:
            tmp_result.file_info_value = convert_to_rpc_FileInfo(value.file_info_value);
            break;
        default:
            LOG_ERR("Unknown type: %d", value.type);
            exit(1);
    }
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

NodeValue convert_from_rpc_NodeValue(Rpc__NodeValue value) {
    assert((ValueType) value.type == (ValueType) value.value_case);

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
            result.string_value = (String) {
                    .value = value.string_value
            };
            break;
        case BOOL:
            result.bool_value = value.bool_value;
            break;
        case FILE_INFO:
            result.file_info_value = convert_from_rpc_FileInfo(*value.file_info_value);
            break;
        default:
            LOG_ERR("Unknown type: %d", value.type);
            exit(1);
    }
    return result;
}

Rpc__CreateFileNodeRequest *convert_to_rpc(CreateFileNodeRequest request) {
    Rpc__NodeId *parent_id = convert_node_id_to_rpc(request.parent_id);

    Rpc__CreateFileNodeRequest *createFileNodeRequest = malloc(sizeof(Rpc__CreateFileNodeRequest));
    Rpc__CreateFileNodeRequest tmp_createFileNodeRequest = RPC__CREATE_FILE_NODE_REQUEST__INIT;
    tmp_createFileNodeRequest.parent_id = parent_id;
    tmp_createFileNodeRequest.file_info = convert_to_rpc_FileInfo(request.file_info);
    *createFileNodeRequest = tmp_createFileNodeRequest;

    return createFileNodeRequest;
}

Rpc__FileInfo *convert_to_rpc_FileInfo(FileInfo file_info) {
    Rpc__FileInfo *rpc_file_info = malloc(sizeof(Rpc__FileInfo));
    Rpc__FileInfo result = RPC__FILE_INFO__INIT;
    result.name = file_info.name;
    result.owner = file_info.owner;
    result.access_time = file_info.access_time;
    result.mime_type = file_info.mime_type;
    *rpc_file_info = result;
    return rpc_file_info;
}

FileInfo convert_from_rpc_FileInfo(Rpc__FileInfo file_info) {
    FileInfo result = {
            .name = file_info.name,
            .owner = file_info.owner,
            .access_time = file_info.access_time,
            .mime_type = file_info.mime_type
    };
    return result;
}