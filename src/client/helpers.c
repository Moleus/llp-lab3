
#include "converters.h"
#include "public/util/memory.h"
#include "helpers.h"
#include "net_client.h"
#include "public/util/log.h"

char *files[] = {
        "ssl root 1705324315 inode/directory",
        "timezone root 1705324315 text/plain",
        "hosts root 1705324315 text/plain",
        "passwd root 1705324315 text/plain",
        "group root 1705324315 text/plain",
        "fstab root 1705324315 text/plain",
        "shadow root 1705324315 text/plain",
        "gshadow root 1705324315 text/plain",
};


// each element has information about parent and file system information
Requests generate_requests() {
    // hardcoded sample data
    // root
    CreateNodeRequest root = {.parent = {.page_id = -1, .item_id = -1}, .value = node_value_string_new("/ root 1705324315 inode/directory")};

    CreateNodeRequest *requests = my_alloc(sizeof(CreateNodeRequest) * 9);
    requests[0] = root;

    for (int i = 0; i < 8; i++) {
        CreateNodeRequest request = {.parent = {.page_id = 0, .item_id = 0}, .value = node_value_string_new(files[i])};
        requests[i+1] = request;
    }

    return (Requests) {
        .count = 9,
        .create = requests
    };
}

// takes last node from path
Rpc__FilterChain* convertNodesToFilterChain(ParsedNode* nodes) {
    ParsedNode *cur_node = nodes;
    int node_names_count = 1;
    while (cur_node != NULL) {
        node_names_count++;
        cur_node = cur_node->next;
    }

    Rpc__FilterChain *filter_chain = my_alloc(sizeof(Rpc__FilterChain));
    Rpc__FilterChain tmp_chain = RPC__FILTER_CHAIN__INIT;
    *filter_chain = tmp_chain;
    filter_chain->n_filters = node_names_count;
    filter_chain->filters = my_alloc(sizeof(Rpc__Filter*) * node_names_count);

    cur_node = my_alloc(sizeof(ParsedNode));
    cur_node->name = "/";
    cur_node->next = nodes;

    int i = 0;
    while (cur_node != NULL) {
        Rpc__Filter *filter = my_alloc(sizeof(Rpc__Filter));
        Rpc__Filter tmp = RPC__FILTER__INIT;
        *filter = tmp;
        filter->type = RPC__FILTER__TYPE__EQUAL;
        filter->argument_case = RPC__FILTER__ARGUMENT_STRING_ARGUMENT;
        filter->string_argument = cur_node->name;
        filter->name = "name";
        filter->field_name_case = RPC__FILTER__FIELD_NAME_NAME;
        filter_chain->filters[i] = filter;
        i++;
        cur_node = cur_node->next;
    }
    return filter_chain;
}

void make_request_based_on_query(Query *q, ClientService *client) {
    switch (q->func) {
        case NOP:
            get_nodes(q, client);
            break;
        case CREATE_OP:
            add_nodes_sequence(q, client);
            break;
        case DELETE_OP:
            break;
        case UPDATE_OP:
            break;
    }
}

void convert_query_args_to_string(Filter *filters, char* buffer) {
    Filter *cur_filter = filters;
    FileInfo fileInfo = {0};

    while (cur_filter != NULL) {
        FilterExpr *expr = cur_filter->filter;
        assert(expr->type == SELECT_BY_LOGICAL_OP);
        assert(expr->operation == EQUALS_OP);
        FileInfoAttributes attr_id = file_info_attribute_from_string(expr->left.name);
        fileInfo = file_info_set_attribute(fileInfo, attr_id, expr->right->string);
        cur_filter = cur_filter->next;
    }
    file_info_to_string(fileInfo, buffer);
}

ParsedNode * parsed_node_get_last(ParsedNode *node) {
    ParsedNode *cur_node = node;
    while (cur_node->next != NULL) {
        cur_node = cur_node->next;
    }
    return cur_node;
}

void add_nodes_sequence(Query *q, ClientService *client) {
    ParsedNode *pNode = q->nodes;
    // get node from server. Store it's id. Set id as a parent for CreateNodeRequest
    // if node doesn't exist then fail

    Rpc__FilterChain *chain = convertNodesToFilterChain(q->nodes);
    Rpc__Nodes *nodes = my_alloc(sizeof(Rpc__Nodes));
    client_get_node_by_filter(client, chain, &nodes);
    assert(nodes != NULL);
    if (nodes->n_nodes == 0) {
        printf("Node doesn't exist\n");
        return;
    }
    node_id_t parent_id = convert_from_rpc_nodeId(nodes->nodes[0]->id);

    char *buffer = my_alloc(256);
    Filter *last_node_filters = parsed_node_get_last(pNode)->filters;
    convert_query_args_to_string(last_node_filters, buffer);

    LOG_INFO("New node values: %s", buffer);
    CreateNodeRequest request = {
        .parent = parent_id,
        .value = node_value_string_new(buffer)
    };

    client_add_node(client, &request);
}

void get_nodes(Query *q, ClientService *client) {
    Rpc__FilterChain *chain = convertNodesToFilterChain(q->nodes);
    Rpc__Nodes result;
    client_get_node_by_filter(client, chain, &result);
}