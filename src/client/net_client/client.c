#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <protobuf-c-rpc/protobuf-c-rpc.h>
#include <unistd.h>
#include "converters.h"
#include "public/util/common.h"
#include "client.h"

#define AUTO_RECONNECT_PERIOD_MS 10

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

static void handle_create_response(const Rpc__CreateFileNodeResponse *response, void *closure_data) {
    printf("handle_create_response\n");
    if (response == NULL) {
        printf("Error processing request.\n");
    } else {
        node_id_t node_id = convert_from_rpc_nodeId(response->node_id);
        printf("Assigned node id: (%d/%d)\n", node_id.page_id, node_id.item_id);
    }
    *(protobuf_c_boolean *) closure_data = 1;
}

void client_add_fileNode(ClientService *self, CreateFileNodeRequest *request) {
    protobuf_c_boolean is_done = 0;
    Rpc__CreateFileNodeRequest *query = convert_to_rpc(*request);

    printf("client_add_fileNode\n");
    rpc__database__create_file_node(self->service, query, handle_create_response, &is_done);
    while (!is_done)
        protobuf_c_rpc_dispatch_run(protobuf_c_rpc_dispatch_default());
}

//void send__create_root_node(ProtobufCService *service) {
//    CreateFileNodeRequest root_node = {
//            .parent_id = {
//                    .page_id = -1,
//                    .item_id = -1
//            },
//            .file_info = {
//                    .name = "root",
//                    .owner = "root",
//                    .access_time = 123456789,
//                    .mime_type = "text/plain"
//            }
//    };
//    send__create_request(service, root_node);
//}

