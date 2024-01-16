#include "common.h"
#include <protobuf-c-rpc/protobuf-c-rpc.h>
#include <malloc.h>
#include "common.pb-c.h"


node_id_t convert_node_id_from_rpc(Rpc__NodeId *node_id) {
    node_id_t result;
    result.page_id = (int32_t) node_id->page_id;
    result.item_id = (int32_t) node_id->item_id;
    return result;
}

Rpc__NodeId convert_node_id_to_rpc(node_id_t node_id) {
    Rpc__NodeId result = RPC__NODE_ID__INIT;
    result.page_id = (uint32_t) node_id.page_id;
    result.item_id = (uint32_t) node_id.item_id;
    return result;
}

CreateFileNodeRequest convert_from_rpc(Rpc__CreateFileNodeRequest request) {
    struct CreateFileNodeRequest createFileNodeRequest;
    createFileNodeRequest.parent_id = convert_node_id_from_rpc(request.parent_id);
    createFileNodeRequest.file_info.name = request.file_info->name;
    createFileNodeRequest.file_info.owner = request.file_info->owner;
    createFileNodeRequest.file_info.access_time = request.file_info->access_time;
    createFileNodeRequest.file_info.mime_type = request.file_info->mime_type;
    return createFileNodeRequest;
}

Rpc__CreateFileNodeRequest convert_to_rpc(CreateFileNodeRequest request) {
    Rpc__CreateFileNodeRequest createFileNodeRequest = RPC__CREATE_FILE_NODE_REQUEST__INIT;
    Rpc__NodeId parent_id = RPC__NODE_ID__INIT;
    parent_id.page_id = (uint32_t) request.parent_id.page_id;
    parent_id.item_id = (uint32_t) request.parent_id.item_id;
    createFileNodeRequest.parent_id = &parent_id;
    Rpc__FileInfo file_info = RPC__FILE_INFO__INIT;
    file_info.name = request.file_info.name;
    file_info.owner = request.file_info.owner;
    file_info.access_time = request.file_info.access_time;
    file_info.mime_type = request.file_info.mime_type;
    createFileNodeRequest.file_info = &file_info;
    return createFileNodeRequest;
}
