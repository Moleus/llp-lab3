#include <protobuf-c-rpc/protobuf-c-rpc.h>
#include <malloc.h>
#include "common.pb-c.h"
#include "converters.h"
#include <signal.h>
#include "public/util/common.h"
#include "public/document_db/document.h"
#include "public/util/node_print.h"

static Document *g_document = NULL;

// Implement server logic here
void handle_create_node_request(const Rpc__CreateNodeRequest *request, Rpc__Node *response) {
    ASSERT_ARG_NOT_NULL(request)
    ASSERT_ARG_NOT_NULL(response)
    ASSERT_ARG_NOT_NULL(request->parent_id)
    ASSERT_ARG_NOT_NULL(request->value)

    CreateNodeRequest node_request = convert_from_rpc_CreateNodeRequest(*request);

    printf("parent_id: (%d/%d)\n", node_request.parent.page_id, node_request.parent.item_id);

    Node *result = malloc(sizeof(Node));

    CreateNodeRequest create_node_request = {
        .parent = node_request.parent,
        .value = node_request.value
    };
    Result res = document_add_node(g_document, &create_node_request, result);
    if (res.status != RES_OK) {
        LOG_ERR("failed to add node: %s", res.message);
        exit(1);
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

    Node *result = malloc(sizeof(Node));

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

    Node *result = malloc(sizeof(Node));

    Result res = document_delete_node(g_document, &node_request, result);
    if (res.status != RES_OK) {
        LOG_ERR("failed to delete node: %s", res.message);
        exit(1);
    }

    *response = *convert_to_rpc_Node(*result);
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

static Rpc__Database_Service databaseService = RPC__DATABASE__INIT(prefix__);

void init_document(const char* filepath, size_t page_size) {
    g_document = document_new();
    Result res = document_init(g_document, filepath, page_size);
    if (res.status != RES_OK) {
        LOG_ERR("failed to init document: %s", res.message);
        exit(1);
    }
}

int main() {
    ProtobufC_RPC_AddressType address_type = PROTOBUF_C_RPC_ADDRESS_TCP;
    const char* filepath = "/tmp/llp-heap-file";
    const char *listen_port = "9090";

    init_document(filepath, 512);

    signal(SIGPIPE, SIG_IGN);

    ProtobufCService *service = (ProtobufCService *) &databaseService;

    protobuf_c_rpc_server_new(address_type, listen_port, service, NULL);

    // Run the server loop
    for (;;) {
        protobuf_c_rpc_dispatch_run(protobuf_c_rpc_dispatch_default());
    }
}
