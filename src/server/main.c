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
    // read from arguments
//    const char* filepath = "/tmp/llp-heap-file";
//    const char *listen_port = "9097";

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <filepath> <port>\n", argv[0]);
        exit(1);
    }

    const char* filepath = argv[1];
    const char *listen_port = argv[2];

    if (listen_port == NULL || filepath == NULL) {
        fprintf(stderr, "Usage: %s <filepath> <port>\n", argv[0]);
        exit(1);
    }

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
