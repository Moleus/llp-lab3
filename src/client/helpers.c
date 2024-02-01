
#include "converters.h"
#include "public/util/memory.h"
#include "helpers.h"

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