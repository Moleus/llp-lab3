#include <protobuf-c-rpc/protobuf-c-rpc.h>
#include <malloc.h>
#include "common.pb-c.h"

// Implement server logic here
void handle_request(const struct Rpc__Request *request, Rpc__Response *response) {
    // Multiply the received number by 2
    response->result = request->number * 2;
}

void prefix__multiply(Rpc__Multiplication_Service *service, const Rpc__Request *input, Rpc__Response_Closure closure, void *closure_data) {
    (void) service;
    // Create a new response object
    Rpc__Response response = RPC__RESPONSE__INIT;
    // Handle the request
    handle_request(input, &response);
    // Call the closure
    closure(&response, closure_data);
}

static Rpc__Multiplication_Service multiplyService = RPC__MULTIPLICATION__INIT(prefix__);

int main() {
    ProtobufC_RPC_AddressType address_type = PROTOBUF_C_RPC_ADDRESS_TCP;
    const char *port = "9090";

    protobuf_c_rpc_server_new(address_type, port, (ProtobufCService *) &multiplyService, NULL);

    // Run the server loop
    for (;;) {
        protobuf_c_rpc_dispatch_run(protobuf_c_rpc_dispatch_default());
    }
    return 0;
}
