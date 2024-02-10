
#include "converters.h"
#include "public/util/memory.h"
#include "helpers.h"
#include "public/util/log.h"
#include "public/util/helpers.h"
#include "../database/include/public/document_db/node.h"

/*
 * {id} {parent_id} {name} {owner} {access_time} {mime_type}
 * file contents:
0 -1 root krot 2024-01-14-20:49:10 inode.directory
1 0 database krot 2024-01-16-13:40:03 inode.directory
2 1 src krot 2024-01-16-13:40:03 inode.directory
3 2 storage krot 2024-01-16-13:40:03 inode.directory
4 3 file.c krot 2024-01-16-13:40:03 text.plain
 */

#define BUF_SIZE 65536

typedef struct {
    int id;
    int parent_id;
    char *file_info_content;
} ParsedEntry;

int count_lines(FILE* file)
{
    char buf[BUF_SIZE];
    int counter = 0;
    for(;;)
    {
        size_t res = fread(buf, 1, BUF_SIZE, file);
        if (ferror(file))
            return -1;

        int i;
        for(i = 0; i < res; i++)
            if (buf[i] == '\n')
                counter++;

        if (feof(file))
            break;
    }

    return counter;
}

ParsedFile read_and_split_by_newline_nodes(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        LOG_ERR("File %s doesn't exist", filename);
        exit(1);
    }

    int count = count_lines(file);

    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *string = my_alloc(fsize + 1);
    fread(string, fsize, 1, file);
    fclose(file);
    string[fsize] = 0;

    char **lines = my_alloc(sizeof(char *) * count);

    char *line = strtok(string, "\n");
    int i = 0;
    while (line != NULL) {
        lines[i] = line;
        line = strtok(NULL, "\n");
        i++;
    }
    return (ParsedFile) {lines, count};
}

void fill_db_from_lines(ClientService *client, char **lines, int count) {
    int nodes_count = count;
    ParsedEntry *all_entries = my_alloc(sizeof(Node) * nodes_count);
    Rpc__Node* added_nodes = my_alloc(sizeof(Rpc__Node) * nodes_count);

    for (int i = 0; i < nodes_count; i++) {
        char *line = lines[i];
        ParsedEntry entry = {0};
        entry.id = atoi(strtok(line, " "));
        entry.parent_id = atoi(strtok(NULL, " "));
        entry.file_info_content = strdup(strtok(NULL, ""));
        all_entries[i] = entry;
    }

    // add root
    assert(all_entries[0].parent_id == -1);
    assert(all_entries[0].id == 0);
    CreateNodeRequest request = {
        .parent = NULL_NODE_ID,
        .value = node_value_string_new(all_entries[0].file_info_content)
    };
    client_add_node(client, &request);
    added_nodes[0] = g_add_node_response;

    for (int i = 1; i < nodes_count; i++) {
        int parent_entry_id = all_entries[i].parent_id;
        node_id_t parent_id = convert_from_rpc_nodeId(added_nodes[parent_entry_id].id);
        CreateNodeRequest request = {
            .parent = parent_id,
            .value = node_value_string_new(all_entries[i].file_info_content)
        };
        client_add_node(client, &request);
        added_nodes[i] = g_add_node_response;
    }
}

Rpc__FilterChain* convertNodesToFilterChainPathOnly(ParsedNode* nodes) {
    ASSERT_ARG_NOT_NULL(nodes);

    ParsedNode *cur_node = nodes;
    int node_names_count = 0;
    node_names_count++; // plus root node
    while (cur_node->next != NULL) {
        node_names_count++;
        cur_node = cur_node->next;
    }

    cur_node = nodes;

    Rpc__FilterChain *filter_chain = my_alloc(sizeof(Rpc__FilterChain));
    Rpc__FilterChain tmp_chain = RPC__FILTER_CHAIN__INIT;
    *filter_chain = tmp_chain;
    filter_chain->n_filters = node_names_count;
    filter_chain->filters = my_alloc(sizeof(Rpc__Filter*) * node_names_count);

    // a/b[*] -> output all children of b
    // a/b -> output b
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

// takes last node from path
Rpc__FilterChain* convertNodesToFilterChain(ParsedNode* nodes) {
    ASSERT_ARG_NOT_NULL(nodes);

    ParsedNode *first_node = nodes;
    ParsedNode *cur_node = nodes;
    int node_names_count = 0;
    node_names_count++; // plus root node
    while (cur_node->next != NULL) {
        node_names_count++;
        cur_node = cur_node->next;
    }

    cur_node = nodes;

    Rpc__FilterChain *filter_chain = my_alloc(sizeof(Rpc__FilterChain));
    Rpc__FilterChain tmp_chain = RPC__FILTER_CHAIN__INIT;
    *filter_chain = tmp_chain;
    filter_chain->n_filters = node_names_count;
    filter_chain->filters = my_alloc(sizeof(Rpc__Filter*) * node_names_count);

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

    // additional filters
    if (first_node->filters != NULL) {
        if (first_node->filters->filter->operation == ALL_OP) {
            Rpc__Filter *filter = my_alloc(sizeof(Rpc__Filter));
            Rpc__Filter tmp = RPC__FILTER__INIT;
            *filter = tmp;
            filter->type = RPC__FILTER__TYPE__ALL;
            filter_chain->filters[i] = filter;
            filter_chain->n_filters++;
        } else {
            Rpc__Filter *filter = my_alloc(sizeof(Rpc__Filter));
            Rpc__Filter tmp = RPC__FILTER__INIT;
            *filter = tmp;
            filter->type = RPC__FILTER__TYPE__FIELD_NAME;
            filter->name = strdup(first_node->filters->filter->left.name);
            filter->string_argument = strdup(first_node->filters->filter->right->string);
            filter->field_name_case = RPC__FILTER__FIELD_NAME_NAME;
            filter->argument_case = RPC__FILTER__ARGUMENT_STRING_ARGUMENT;
            filter_chain->filters[i] = filter;
            filter_chain->n_filters++;
        }
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
            delete_node_sequence(q, client);
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

    Rpc__FilterChain *chain = convertNodesToFilterChainPathOnly(q->nodes);
    // TODO: get returned node
    client_get_node_by_filter(client, chain);
    Rpc__Nodes nodes = g_get_nodes_response;
    if (nodes.n_nodes == 0) {
        LOG_WARN("Node doesn't exist\n", "");
        return;
    }
    node_id_t parent_id = convert_from_rpc_nodeId(nodes.nodes[0]->id);

    char *buffer = my_alloc(256);
    Filter *last_node_filters = pNode->filters;
    convert_query_args_to_string(last_node_filters, buffer);

    LOG_INFO("New node values: %s", buffer);
    CreateNodeRequest request = {
        .parent = parent_id,
        .value = node_value_string_new(buffer)
    };

    client_add_node(client, &request);
}

void delete_node_sequence(Query *q, ClientService *client) {
    Rpc__FilterChain *chain = convertNodesToFilterChain(q->nodes);
//    client_get_node_by_filter(client, chain);
//    Rpc__Nodes nodes = g_get_nodes_response;
//    if (nodes.n_nodes == 0) {
//        LOG_WARN("Node doesn't exist\n", "");
//        return;
//    }
//    node_id_t node_id = convert_from_rpc_nodeId(nodes.nodes[0]->id);
//    DeleteNodeRequest request = {
//        .node_id = node_id
//    };
//    client_delete_node(client, &request);
    client_delete_node_by_filter(client, chain);
}

void get_nodes(Query *q, ClientService *client) {
    Rpc__FilterChain *chain = convertNodesToFilterChain(q->nodes);
    Rpc__Nodes result;
    client_get_node_by_filter(client, chain);
}