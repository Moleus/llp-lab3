#include <protobuf-c-rpc/protobuf-c-rpc.h>
#include <malloc.h>
#include "common.pb-c.h"
#include "common.h"
#include <signal.h>

// Implement server logic here
void handle_create_node_request(const Rpc__CreateFileNodeRequest *request, Rpc__CreateFileNodeResponse *response) {
    Rpc__NodeId *rpcNodeId = malloc(sizeof(Rpc__NodeId));
    // hardcode data
    node_id_t node_id = {
        .page_id = 1,
        .item_id = 2
    };

    printf("Sending node id: (%d/%d)\n", node_id.page_id, node_id.item_id);

    *rpcNodeId = convert_node_id_to_rpc(node_id);
    response->node_id = rpcNodeId;
}

void prefix__create_file_node(Rpc__Database_Service *service, const Rpc__CreateFileNodeRequest *input, Rpc__CreateFileNodeResponse_Closure closure, void *closure_data) {
    (void) service;
    printf("backend__create_file_node\n");
    Rpc__CreateFileNodeResponse response = RPC__CREATE_FILE_NODE_RESPONSE__INIT;
    handle_create_node_request(input, &response);
    closure(&response, closure_data);
}

static Rpc__Database_Service databaseService = RPC__DATABASE__INIT(prefix__);

int main() {
    ProtobufC_RPC_AddressType address_type = PROTOBUF_C_RPC_ADDRESS_TCP;
    const char *port = "9090";

    signal(SIGPIPE, SIG_IGN);

    ProtobufCService *service = (ProtobufCService *) &databaseService;

    protobuf_c_rpc_server_new(address_type, port, service, NULL);

    // Run the server loop
    for (;;) {
        protobuf_c_rpc_dispatch_run(protobuf_c_rpc_dispatch_default());
    }
}
