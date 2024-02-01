#include <protobuf-c-rpc/protobuf-c-rpc.h>
#include <malloc.h>
#include "common.pb-c.h"
#include "converters.h"
#include <signal.h>
#include "public/util/common.h"
#include "public/document_db/document.h"
#include "public/structures.h"
#include "fs.h"
#include "public/util/memory.h"
#include "server.h"

unsigned char log_level = INFO;
extern ProtobufCService databaseService;

int main(int argc, char **argv) {
    ProtobufC_RPC_AddressType address_type = PROTOBUF_C_RPC_ADDRESS_TCP;
    const char* filepath = "/tmp/llp-heap-file";
    const char *listen_port = "9096";

    remove(filepath);

    server_init_document(filepath, 512);

    signal(SIGPIPE, SIG_IGN);

    ProtobufCService *service = (ProtobufCService *) &databaseService;

    // TODO: fix address already in use
    ProtobufC_RPC_Server *pServer = protobuf_c_rpc_server_new(address_type, listen_port, service, NULL);
    if (pServer == NULL) {
        fprintf(stderr, "Error creating server!\n");
        exit(1);
    }

    // Run the server loop
    for (;;) {
        protobuf_c_rpc_dispatch_run(protobuf_c_rpc_dispatch_default());
    }
}
