#include <malloc.h>
#include "common.pb-c.h"
#include "converters.h"
#include "public/util/common.h"
#include "public/document_db/document.h"
#include "public/structures.h"
#include "fs.h"
#include "public/util/memory.h"
#include "server.h"

static Document *g_document = NULL;

void _set_document(Document *document) {
    g_document = document;
}

// Implement server logic here
void handle_create_node_request(const Rpc__CreateNodeRequest *request, Rpc__Node *response) {
    ASSERT_ARG_NOT_NULL(request)
    ASSERT_ARG_NOT_NULL(response)
    ASSERT_ARG_NOT_NULL(request->parent_id)
    ASSERT_ARG_NOT_NULL(request->value)

    CreateNodeRequest node_request = convert_from_rpc_CreateNodeRequest(*request);

    Node *result = my_alloc(sizeof(Node));

    CreateNodeRequest create_node_request = {
        .parent = node_request.parent,
        .value = node_request.value
    };
    LOG_INFO("[handler] create node request. parent: (%d/%d)", create_node_request.parent.page_id, create_node_request.parent.item_id);
    Result res = document_add_node(g_document, &create_node_request, result);
    if (res.status != RES_OK) {
        LOG_ERR("[handler] failed to add node: %s", res.message);
    }
    // TODO: pointer or value? is it initialized?
    *response = *convert_to_rpc_Node(*result);
}

void handle_update_node_request(const Rpc__UpdateNodeRequest *request, Rpc__Node *response) {
    ASSERT_ARG_NOT_NULL(request)
    ASSERT_ARG_NOT_NULL(response)
    ASSERT_ARG_NOT_NULL(request->id)
    ASSERT_ARG_NOT_NULL(request->value)

    UpdateNodeRequest node_request = convert_from_rpc_UpdateNodeRequest(*request);

    Node *result = my_alloc(sizeof(Node));

    LOG_INFO("[handler] update node request. id: (%d/%d)", node_request.node_id.page_id, node_request.node_id.item_id);
    Result res = document_update_node(g_document, &node_request, result);
    if (res.status != RES_OK) {
        LOG_ERR("failed to update node: %s", res.message);
        exit(1);
    }

    *response = *convert_to_rpc_Node(*result);
}

void handle_delete_node_request(const Rpc__DeleteNodeRequest *request, Rpc__Node *response) {
    ASSERT_ARG_NOT_NULL(request)
    ASSERT_ARG_NOT_NULL(response)
    ASSERT_ARG_NOT_NULL(request->id)

    DeleteNodeRequest node_request = convert_from_rpc_DeleteNodeRequest(*request);

    Node *result = my_alloc(sizeof(Node));

    Result res = document_delete_node(g_document, &node_request, result);
    if (res.status != RES_OK) {
        LOG_ERR("failed to delete node: %s", res.message);
        exit(1);
    }

    *response = *convert_to_rpc_Node(*result);
}

void get_nodes_by_filter_request(const Rpc__FilterChain *request, Rpc__Nodes *response) {
    ASSERT_ARG_NOT_NULL(request)
    ASSERT_ARG_NOT_NULL(response)

    NodeMatcherArray *matcherArray = fs_new_node_matcher_array(request);

    int nodes_count = 0;
    // TODO: returns 0 on /ssl when not restarted
    Result res = document_count_nodes_by_condition_sequence(g_document, matcherArray, &nodes_count);
    if (res.status != RES_OK) {
        LOG_ERR("[handler] failed to count nodes by filter: %s", res.message);
    }
    LOG_INFO("[handler] nodes count: %d", nodes_count);

    if (nodes_count == 0) {
        Rpc__Nodes tmp_result = RPC__NODES__INIT;
        *response = tmp_result;
        return;
    }

    NodesArray *result = nodes_array_new(nodes_count);

    result->count = nodes_count;
    res = document_get_nodes_by_condition_sequence(g_document, matcherArray, result);
    if (res.status != RES_OK) {
        result->count = 0;
        LOG_ERR("[handler] failed to get node by filter: %s", res.message);
    }

    *response = convert_to_rpc_Nodes(result);
}

void handle_get_nodes_by_filter_request(const Rpc__FilterChain *request, Rpc__Nodes *response) {
    get_nodes_by_filter_request(request, response);
}

void handle_delete_nodes_by_filter_request(const Rpc__FilterChain *request, Rpc__DeletedNodes *response) {
    Rpc__Nodes nodes = RPC__NODES__INIT;

    get_nodes_by_filter_request(request, &nodes);
    if (nodes.n_nodes == 0) {
        Rpc__DeletedNodes tmp_result = RPC__DELETED_NODES__INIT;
        *response = tmp_result;
        return;
    }

    for (int i = 0; i < nodes.n_nodes; i++) {
        Rpc__Node *node = nodes.nodes[i];
        node_id_t delete_node_id = convert_from_rpc_nodeId(node->id);
        int tmp_count;
        Result res = delete_node_with_all_descendants(g_document, delete_node_id, &tmp_count);
        if (res.status != RES_OK) {
            LOG_ERR("failed to delete node: %s", res.message);
            exit(1);
        }
    }
    LOG_INFO("[handler] deleted nodes count: %d", nodes.n_nodes);
    Rpc__DeletedNodes tmp_result = RPC__DELETED_NODES__INIT;
    tmp_result.count = nodes.n_nodes;
    *response = tmp_result;
}

void prefix__create_node(Rpc__Database_Service *service, const Rpc__CreateNodeRequest *input, Rpc__Node_Closure closure, void *closure_data) {
    (void) service;
    Rpc__Node response = RPC__NODE__INIT;
    handle_create_node_request(input, &response);
    closure(&response, closure_data);
}

void prefix__update_node(Rpc__Database_Service *service, const Rpc__UpdateNodeRequest *input, Rpc__Node_Closure closure, void *closure_data) {
    (void) service;
    Rpc__Node response = RPC__NODE__INIT;
    handle_update_node_request(input, &response);
    closure(&response, closure_data);
}

void prefix__delete_node(Rpc__Database_Service *service, const Rpc__DeleteNodeRequest *input, Rpc__Node_Closure closure, void *closure_data) {
    (void) service;
    Rpc__Node response = RPC__NODE__INIT;
    handle_delete_node_request(input, &response);
    closure(&response, closure_data);
}

void prefix__get_node(Rpc__Database_Service *service, const Rpc__NodeId *input, Rpc__Node_Closure closure, void *closure_data) {
    (void) service;
    (void) input;
    (void) closure;
    (void) closure_data;
}

void prefix__get_nodes_by_filter(Rpc__Database_Service *service, const Rpc__FilterChain *input, Rpc__Nodes_Closure closure, void *closure_data) {
    (void) service;
    LOG_INFO("[rpc__node_by_filter] Received get_node_by_filter request. Filters count: %d", input->n_filters);
    Rpc__Nodes response = RPC__NODES__INIT;
    handle_get_nodes_by_filter_request(input, &response);
    closure(&response, closure_data);
}

void prefix__delete_nodes_by_filter(Rpc__Database_Service *service, const Rpc__FilterChain *input, Rpc__DeletedNodes_Closure closure, void *closure_data) {
    (void) service;
    LOG_INFO("[rpc__node_by_filter] Received delete_nodes_by_filter request. Filters count: %d", input->n_filters);
    Rpc__DeletedNodes response = RPC__DELETED_NODES__INIT;
    handle_delete_nodes_by_filter_request(input, &response);
    closure(&response, closure_data);
}

Rpc__Database_Service databaseService = RPC__DATABASE__INIT(prefix__);

Document *server_init_document(const char* filepath, size_t page_size) {
    g_document = document_new();
    Result res = document_init(g_document, filepath, page_size);
    if (res.status != RES_OK) {
        LOG_ERR("failed to init document: %s", res.message);
        exit(1);
    }
    return g_document;
}

