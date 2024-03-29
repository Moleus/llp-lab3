#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <protobuf-c-rpc/protobuf-c-rpc.h>
#include <unistd.h>
#include "converters.h"
#include "public/util/common.h"
#include "net_client.h"

#define AUTO_RECONNECT_PERIOD_MS 10


Rpc__Nodes g_get_nodes_response = {0};
Rpc__Node g_add_node_response = {0};

static void die(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}

struct ClientService {
    ProtobufCService *service;
};

ClientService *client_service_new(char *address) {
    ClientService *self = malloc(sizeof(ClientService));
    ProtobufC_RPC_Client *client;
    ProtobufCService *service = protobuf_c_rpc_client_new(PROTOBUF_C_RPC_ADDRESS_TCP, address, &rpc__database__descriptor,
                                        NULL);
    if (service == NULL)
        die("Error creating client");

    client = (ProtobufC_RPC_Client *) service;

    protobuf_c_rpc_client_set_autoreconnect_period(client, AUTO_RECONNECT_PERIOD_MS);

    fprintf(stderr, "Connecting... ");
    while (!protobuf_c_rpc_client_is_connected(client))
        protobuf_c_rpc_dispatch_run(protobuf_c_rpc_dispatch_default());
    fprintf(stderr, "done.\n");

    self->service = service;
    return self;
}

static void do_nothing(ProtobufCRPCDispatch *dispatch, void *unused) {
}

static void run_main_loop_without_blocking(ProtobufCRPCDispatch *dispatch) {
    protobuf_c_rpc_dispatch_add_idle(dispatch, do_nothing, NULL);
    protobuf_c_rpc_dispatch_run(dispatch);
}

void run_main_loop(ClientService *self) {
    run_main_loop_without_blocking(protobuf_c_rpc_dispatch_default());
}

static void handle_create_response(const Rpc__Node *response, void *closure_data) {
    LOG_INFO("handle_create_response\n", "");
    if (response == NULL) {
        LOG_WARN("Error processing request.\n", "");
    } else {
        g_add_node_response = converters_copy_node(*response);
        node_id_t node_id = convert_from_rpc_nodeId(response->id);
        LOG_INFO("Assigned node id: (%d/%d)\n", node_id.page_id, node_id.item_id);
        if (node_id.item_id != -1) {
            Rpc__NodeValue *nodeValue = response->value;
            if (nodeValue->type != RPC__NODE_VALUE__TYPE__STRING) {
                LOG_ERR("Error: node value is not string. Type: %d\n", nodeValue->type);
                exit(1);
            } else {
                printf("Data: %s\n", nodeValue->string_value);
            }
        }
    }
    *(protobuf_c_boolean *) closure_data = 1;
}

static void handle_get_response(const Rpc__Nodes *response, void *closure_data) {
    printf("Handling server response...\n");
    if (response == NULL) {
        printf("Error processing request.\n");
    } else {
        printf("    Nodes satisfy your request: %zu\n", response->n_nodes);
        for (size_t i = 0; i < response->n_nodes; i++) {
            Rpc__Node *node = response->nodes[i];
            node_id_t node_id = convert_from_rpc_nodeId(node->id);
            printf("    Node id: (%d/%d)\n", node_id.page_id, node_id.item_id);
            Rpc__NodeValue *nodeValue = node->value;
            printf("    Data: %s\n", nodeValue->string_value);
        }
    }
    g_get_nodes_response = converters_copy_nodes(*response);
    Closure *closure = (Closure*) closure_data;
    closure->is_done = 1;
}

static void handle_delete_nodes_response(const Rpc__DeletedNodes *response, void *closure_data) {
    LOG_DEBUG("handle_delete_nodes_response\n", "");
    if (response == NULL) {
        printf("Error processing request.\n");
    } else {
        printf("Successfully deleted nodes: %d\n", response->count);
    }
    *(protobuf_c_boolean *) closure_data = 1;
}

void client_add_node(ClientService *self, CreateNodeRequest *request) {
    protobuf_c_boolean is_done = 0;
    Rpc__CreateNodeRequest *query = convert_to_rpc_CreateNodeRequest(*request);

    rpc__database__create_node(self->service, query, handle_create_response, &is_done);
    while (!is_done)
        protobuf_c_rpc_dispatch_run(protobuf_c_rpc_dispatch_default());
}

void client_get_node_by_filter(ClientService *self, Rpc__FilterChain *filters) {
    Closure closure = {0};
    rpc__database__get_nodes_by_filter(self->service, filters, handle_get_response, &closure);
    while (!closure.is_done)
        protobuf_c_rpc_dispatch_run(protobuf_c_rpc_dispatch_default());
}

void client_delete_node_by_filter(ClientService *self, Rpc__FilterChain *filters) {
    protobuf_c_boolean is_done = 0;
    rpc__database__delete_nodes_by_filter(self->service, filters, handle_delete_nodes_response, &is_done);
    while (!is_done)
        protobuf_c_rpc_dispatch_run(protobuf_c_rpc_dispatch_default());
}

void client_delete_all_nodes(ClientService *self) {
    protobuf_c_boolean is_done = 0;
    printf("client_delete_all_nodes\n");
    Rpc__FilterChain chain = RPC__FILTER_CHAIN__INIT;
    rpc__database__delete_nodes_by_filter(self->service, &chain, handle_delete_nodes_response, &is_done);
    while (!is_done)
        protobuf_c_rpc_dispatch_run(protobuf_c_rpc_dispatch_default());
}