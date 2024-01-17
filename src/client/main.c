#include <unistd.h>
#include "net_client.h"
#include "public/util/log.h"
#include "parser/types.h"
#include "parser/my_parser.h"
#include "public/structures.h"
#include "public/util/memory.h"

unsigned char log_level = DEBUG;

// Convert Query to CreateNodeRequest/UpdateNodeRequest/DeleteNodeRequest
//void convert_query_to_request(Query *query) {
//    void* handler = client_get_node;
//    switch (query->func) {
//        case CREATE_OP:
//            handler = client_add_node;
//            break;
//        case UPDATE_OP:
//            handler = client_update_node;
//            break;
//        case DELETE_OP:
//            handler = client_delete_node;
//            break;
//        case NOP:
//            handler = client_get_node;
//            break;
//        default:
//            LOG_ERR("Unknown function type: %d", query->func);
//            exit(1);
//    }
//    ParsedNode *node = query->nodes;
//}

Rpc__FilterChain* convertNodesToFilterChain(ParsedNode* nodes) {
    ParsedNode *cur_node = nodes;
    int node_names_count = 0;
    while (cur_node != NULL) {
        node_names_count++;
        cur_node = cur_node->next;
    }

    Rpc__FilterChain *filter_chain = my_alloc(sizeof(Rpc__FilterChain));
    Rpc__FilterChain tmp_chain = RPC__FILTER_CHAIN__INIT;
    *filter_chain = tmp_chain;
    filter_chain->n_filters = node_names_count;
    filter_chain->filters = my_alloc(sizeof(Rpc__Filter*) * node_names_count);

    cur_node = nodes;
    int i = 0;
    while (cur_node != NULL) {
        Rpc__Filter *filter = my_alloc(sizeof(Rpc__Filter));
        Rpc__Filter tmp = RPC__FILTER__INIT;
        *filter = tmp;
        filter->type = RPC__FILTER__TYPE__EQUAL;
        filter->argument_case = RPC__FILTER__ARGUMENT_STRING_ARGUMENT;
        filter->string_argument = cur_node->name;
        filter_chain->filters[i] = filter;
        i++;
        cur_node = cur_node->next;
    }
    return filter_chain;
}

// read from input using parser
// then call net_client functions based on the query
int main(int argc, char **argv) {
    char *address = "127.0.0.1:9090";

    char *command = "test.txt\n";
    Query *q = parser_parse_command(command);

    Rpc__FilterChain *chain = convertNodesToFilterChain(q->nodes);

    ClientService *service = client_service_new(address);

    for (;;) {
//        Query query = parse();

        // hardcoded sample data
        CreateNodeRequest data = {
                .parent = {
                        .page_id = 0,
                        .item_id = 0
                },
                .value = (NodeValue) {
                        .type = FILE_INFO,
                        .file_info_value = {
                                .name = "test.txt",
                                .owner = "test",
                                .access_time = 123456789,
                                .mime_type = "text/plain"
                        }
                }
        };

        run_main_loop(service);

        client_add_node(service, &data);
        client_get_node_by_filter(service, chain);
        sleep(5);
    }
}