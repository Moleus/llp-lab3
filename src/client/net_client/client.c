#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <protobuf-c-rpc/protobuf-c-rpc.h>
#include <unistd.h>
#include "common.h"
#include "public/util/common.h"

#define AUTO_RECONNECT_PERIOD_MS 10

unsigned char log_level = DEBUG;

static void die(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}

/* Run the main-loop without blocking.  It would be nice
   if there was a simple API for this (protobuf_c_rpc_dispatch_run_with_timeout?),
   but there isn't for now. */
static void do_nothing(ProtobufCRPCDispatch *dispatch, void *unused) {
}

static void run_main_loop_without_blocking(ProtobufCRPCDispatch *dispatch) {
    protobuf_c_rpc_dispatch_add_idle(dispatch, do_nothing, NULL);
    protobuf_c_rpc_dispatch_run(dispatch);
}

ProtobufCService *my_connect(char * address) {
    ProtobufC_RPC_Client *client;
    ProtobufCService *service;
    service = protobuf_c_rpc_client_new(PROTOBUF_C_RPC_ADDRESS_TCP, address, &rpc__database__descriptor,
                                        NULL);
    if (service == NULL)
        die("Error creating client");

    client = (ProtobufC_RPC_Client *) service;

    protobuf_c_rpc_client_set_autoreconnect_period(client, AUTO_RECONNECT_PERIOD_MS);

    fprintf(stderr, "Connecting... ");
    while (!protobuf_c_rpc_client_is_connected(client))
        protobuf_c_rpc_dispatch_run(protobuf_c_rpc_dispatch_default());
    fprintf(stderr, "done.\n");
    return service;
}


static void handle_create_response(const Rpc__CreateFileNodeResponse *response, void *closure_data) {
    printf("handle_create_response\n");
    if (response == NULL) {
        printf("Error processing request.\n");
    } else {
        node_id_t node_id = convert_node_id_from_rpc(response->node_id);
        printf("Assigned node id: (%d/%d)\n", node_id.page_id, node_id.item_id);
    }
    *(protobuf_c_boolean *) closure_data = 1;
}

void send__create_request(ProtobufCService *service, CreateFileNodeRequest createFileNode) {
    protobuf_c_boolean is_done = 0;
    Rpc__CreateFileNodeRequest *query = convert_to_rpc(createFileNode);

    printf("send__create_request\n");
    rpc__database__create_file_node(service, query, handle_create_response, &is_done);
    while (!is_done)
        protobuf_c_rpc_dispatch_run(protobuf_c_rpc_dispatch_default());
}

int main(int argc, char **argv) {
    char *address = "127.0.0.1:9090";
    ProtobufCService *service = my_connect(address);

    for (;;) {
        // hardcoded sample data
        CreateFileNodeRequest data = {
            .parent_id = {
                .page_id = 1,
                .item_id = 2
            },
            .file_info = {
                .name = "test.txt",
                .owner = "test",
                .access_time = 123456789,
                .mime_type = "text/plain"
            }
        };

        // hardcoded root note parent_id = {-1, -1}
        CreateFileNodeRequest root_node = {
            .parent_id = {
                .page_id = -1,
                .item_id = -1
            },
            .file_info = {
                .name = "root",
                .owner = "root",
                .access_time = 123456789,
                .mime_type = "text/plain"
            }
        };

        run_main_loop_without_blocking(protobuf_c_rpc_dispatch_default());

        // send root node
        send__create_request(service, root_node);
        printf("root node sent\n");
        // sleep 3 seconds
//        sleep(5);
//        send__create_request(service, data);
//        printf("data sent\n");
        // sleep 3 seconds
        sleep(3);
    }
}

