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
    char *address = "127.0.0.1:9097";

    ClientService *service = client_service_new(address);

    // setup
    client_delete_all_nodes(service);
    fill_with_data(service);

    char *command_template = "create(ssl[@name=a%d][@owner=root][@access_time=1705324315][@mime_type=text/plain])\n";
    char *get_cmd = "ssl[*]\n";
    Query *get_cmd_q = parser_parse_command(get_cmd);
    char *command = malloc(100);
    for (int i = 0; i < 100; ++i) {
        run_main_loop(service);

        sprintf(command, command_template, i);
        Query *query = parser_parse_command(command);
        make_request_based_on_query(query, service);
        get_nodes(get_cmd_q, service);

        sleep(5);
    }
}