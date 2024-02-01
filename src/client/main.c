#include <unistd.h>
#include "net_client.h"
#include "public/util/log.h"
#include "parser/types.h"
#include "parser/my_parser.h"
#include "helpers.h"

unsigned char log_level = DEBUG;


void fill_with_data(ClientService *client) {
    Requests r = generate_requests();

    for (int i = 0; i < r.count; ++i) {
        client_add_node(client, &r.create[i]);
    }
}

// read from input using parser
// then call net_client functions based on the query
int main(int argc, char **argv) {
    char *address = "127.0.0.1:9096";

    char *command = "hosts\n";
    Query *q = parser_parse_command(command);
//
    Rpc__FilterChain *chain = convertNodesToFilterChain(q->nodes);

    ClientService *service = client_service_new(address);

    client_delete_all_nodes(service);
    fill_with_data(service);
    for (;;) {
//        Query query = parse();

        run_main_loop(service);

        client_get_node_by_filter(service, chain);
        sleep(5);
    }
}