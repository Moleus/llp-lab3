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

    ClientService *service = client_service_new(address);

    // setup
    client_delete_all_nodes(service);
    fill_with_data(service);

    for (;;) {
        char *command = "create(ssl[name=a1][owner=root][access_time=1705324315][mime_type=text/plain])\n";
        Query *query = parser_parse_command(command);
        run_main_loop(service);
        make_request_based_on_query(query, service);
        sleep(5);
    }
}