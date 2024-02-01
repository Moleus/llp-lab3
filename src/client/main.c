#include <unistd.h>
#include "net_client.h"
#include "public/util/log.h"
#include "parser/types.h"
#include "parser/my_parser.h"
#include "public/util/memory.h"
#include "helpers.h"

unsigned char log_level = DEBUG;

// takes last node from path
Rpc__FilterChain* convertNodesToFilterChain(ParsedNode* nodes) {
    ParsedNode *last_node = nodes;
    int node_names_count = 0;
    while (last_node->next != NULL) {
        last_node = last_node->next;
        node_names_count++;
    }

    Rpc__FilterChain *filter_chain = my_alloc(sizeof(Rpc__FilterChain));
    Rpc__FilterChain tmp_chain = RPC__FILTER_CHAIN__INIT;
    *filter_chain = tmp_chain;
    filter_chain->n_filters = 1;
    filter_chain->filters = my_alloc(sizeof(Rpc__Filter*) * 1);

    Rpc__Filter *filter = my_alloc(sizeof(Rpc__Filter));
    Rpc__Filter tmp = RPC__FILTER__INIT;
    *filter = tmp;
    filter->type = RPC__FILTER__TYPE__EQUAL;
    filter->argument_case = RPC__FILTER__ARGUMENT_STRING_ARGUMENT;
    filter->string_argument = last_node->name;
    filter_chain->filters[0] = filter;
    return filter_chain;
}

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

    char *command = "/hosts\n";
    Query *q = parser_parse_command(command);
//
    Rpc__FilterChain *chain = convertNodesToFilterChain(q->nodes);

    ClientService *service = client_service_new(address);

    client_delete_all_nodes(service);
    for (;;) {
//        Query query = parse();

        run_main_loop(service);

        client_get_node_by_filter(service, chain);
        sleep(5);
    }
}