#include <unistd.h>
#include "net_client.h"
#include "public/util/log.h"
#include "parser/types.h"
#include "parser/my_parser.h"
#include "public/structures.h"

unsigned char log_level = DEBUG;

/*
typedef struct {
    LogicalOperation operation;
    FilterTarget left;
    Element *right;
    FilterExprType type;
    bool is_single_value;
} FilterExpr;

typedef struct {
    ParsedValueType type;
    char *name;
} Property;

typedef struct Filter {
    union {
        Property* property; // when adding new property
        FilterExpr* filter; // when querying
    };
    struct Filter *next;
} Filter;

typedef enum {
    SINGLE_ATTRIBUTE = 1,
    ASTERISK_PATH,
} PathType;

typedef struct ParsedNode {
    char *name;
    struct ParsedNode *next;
    Filter* filters;
} ParsedNode;

typedef struct {
    FunctionType func;
    ParsedNode *nodes;
} Query;
 */


// Convert Query to CreateNodeRequest/UpdateNodeRequest/DeleteNodeRequest
void convert_query_to_request(Query *query) {
    void* handler = client_get_node;
    switch (query->func) {
        case CREATE_OP:
            handler = client_add_node;
            break;
        case UPDATE_OP:
            handler = client_update_node;
            break;
        case DELETE_OP:
            handler = client_delete_node;
            break;
        case NOP:
            handler = client_get_node;
            break;
        default:
            LOG_ERR("Unknown function type: %d", query->func);
            exit(1);
    }
    ParsedNode *node = query->nodes;
}


// read from input using parser
// then call net_client functions based on the query
int main(int argc, char **argv) {
    char *address = "127.0.0.1:9090";

    char *command = "a/b/d[@x=1][@y=2][@z=hello]\n";
    Query *q = parser_parse_command(command);
    return 0;

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
        sleep(5);
    }
}