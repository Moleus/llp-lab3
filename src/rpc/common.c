#include "common.h"
#include <malloc.h>
#include "common.pb-c.h"
#include "public/util/common.h"


node_id_t convert_node_id_from_rpc(Rpc__NodeId *node_id) {
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

CreateFileNodeRequest convert_from_rpc(Rpc__CreateFileNodeRequest request) {
    struct CreateFileNodeRequest createFileNodeRequest;
    createFileNodeRequest.parent_id = convert_node_id_from_rpc(request.parent_id);
    createFileNodeRequest.file_info.name = request.file_info->name;
    createFileNodeRequest.file_info.owner = request.file_info->owner;
    createFileNodeRequest.file_info.access_time = request.file_info->access_time;
    createFileNodeRequest.file_info.mime_type = request.file_info->mime_type;
    return createFileNodeRequest;
}

Rpc__CreateFileNodeRequest *convert_to_rpc(CreateFileNodeRequest request) {
    Rpc__NodeId *parent_id = convert_node_id_to_rpc(request.parent_id);

    Rpc__FileInfo *file_info = malloc(sizeof(Rpc__FileInfo));
    Rpc__FileInfo tmp_file_info = convert_file_info_to_rpc(request.file_info);
    *file_info = tmp_file_info;

    Rpc__CreateFileNodeRequest *createFileNodeRequest = malloc(sizeof(Rpc__CreateFileNodeRequest));
    Rpc__CreateFileNodeRequest tmp_createFileNodeRequest = RPC__CREATE_FILE_NODE_REQUEST__INIT;
    tmp_createFileNodeRequest.parent_id = parent_id;
    tmp_createFileNodeRequest.file_info = file_info;
    *createFileNodeRequest = tmp_createFileNodeRequest;

    return createFileNodeRequest;
}

Rpc__FileInfo convert_file_info_to_rpc(FileInfo file_info) {
    Rpc__FileInfo result = RPC__FILE_INFO__INIT;
    result.name = file_info.name;
    result.owner = file_info.owner;
    result.access_time = file_info.access_time;
    result.mime_type = file_info.mime_type;
    return result;
}
