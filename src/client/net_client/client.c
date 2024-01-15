#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <protobuf-c-rpc/protobuf-c-rpc.h>
#include "common.pb-c.h"

#define AUTO_RECONNECT_PERIOD_MS 10

static void die(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}

static void handle_response(const Rpc__Response *response, void *closure_data) {
    if (response == NULL) {
        printf("Error processing request.\n");
    } else {
        printf("Server response: %d\n", response->result);
    }
    *(protobuf_c_boolean *) closure_data = 1;
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

int main(int argc, char **argv) {
    ProtobufC_RPC_Client *client;
    ProtobufCService *service;

    // Connect to the server
    service = protobuf_c_rpc_client_new(PROTOBUF_C_RPC_ADDRESS_TCP, "127.0.0.1:9090", &rpc__multiplication__descriptor,
                                        NULL);
    if (service == NULL)
        die("Error creating client");

    client = (ProtobufC_RPC_Client *) service;

    protobuf_c_rpc_client_set_autoreconnect_period(client, AUTO_RECONNECT_PERIOD_MS);

    fprintf(stderr, "Connecting... ");
    while (!protobuf_c_rpc_client_is_connected(client))
        protobuf_c_rpc_dispatch_run(protobuf_c_rpc_dispatch_default());
    fprintf(stderr, "done.\n");

    for (;;) {
        char buf[1024];
        Rpc__Request query = RPC__REQUEST__INIT;
        protobuf_c_boolean is_done = 0;
        fprintf(stderr, ">> ");
        if (fgets(buf, sizeof(buf), stdin) == NULL)
            break;

        /* In order to prevent having the client get unduly stuck
           in an error state, exercise the main-loop, which will
           give the connection process time to run. */
        run_main_loop_without_blocking(protobuf_c_rpc_dispatch_default());

        query.number = atoi(buf);
        rpc__multiplication__multiply(service, &query, handle_response, &is_done);
        while (!is_done)
            protobuf_c_rpc_dispatch_run(protobuf_c_rpc_dispatch_default());
    }
    return 0;
}
