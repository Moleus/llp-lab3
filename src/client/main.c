#include <unistd.h>
#include "net_client.h"
#include "public/util/log.h"
#include "parser/types.h"
#include "parser/my_parser.h"
#include "helpers.h"

unsigned char log_level = DEBUG;


void fill_with_data(ClientService *client, const char *init_file) {
    ParsedFile f = read_and_split_by_newline_nodes(init_file);
    fill_db_from_lines(client, f.lines, f.count);
}

// read from input using parser
// then call net_client functions based on the query
int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <address>\n", argv[0]);
        exit(1);
    }

    const char *address = argv[1];
    const char *init_file = "/tmp/init.txt";

    if (address == NULL) {
        fprintf(stderr, "Usage: %s <address>\n", argv[0]);
        exit(1);
    }

    ClientService *service = client_service_new(address);

//    char *command_template = "create(ssl[@name=subssl][@owner=root][@access_time=1705324315][@mime_type=text/plain])\n";
//    char *get_cmd = "ssl[*]\n";
//    Query *get_cmd_q = parser_parse_command(get_cmd);

    char command[100]; // Changed to an array from malloc
    while(1) {
        printf("Enter your query: ");
        fflush(stdout); // Ensure prompt is displayed before input

        // Read from input
        fgets(command, sizeof(command), stdin);

        if (strcmp(command, "exit\n") == 0) {
            break;
        }

        if (strcmp(command, "init\n") == 0) {
            client_delete_all_nodes(service);
            fill_with_data(service, init_file);
            continue;
        }
        // Parse and process command
        Query *query = parser_parse_command(command);
        make_request_based_on_query(query, service);
//        get_nodes(get_cmd_q, service);

        // Sleep to control loop speed
//        sleep(5);
    }
}