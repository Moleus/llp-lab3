#pragma once

#include "parser/types.h"
#include "converters.h"
#include "net_client.h"

typedef struct {
    char **lines;
    int count;
} ParsedFile;

ParsedFile read_and_split_by_newline_nodes(const char *filename);
void fill_db_from_lines(ClientService *client, char **lines, int count);

Rpc__FilterChain* convertNodesToFilterChain(ParsedNode* nodes);

void make_request_based_on_query(Query *q, ClientService *client);

void add_nodes_sequence(Query *q, ClientService *client);

void delete_node_sequence(Query *q, ClientService *client);

void get_nodes(Query *q, ClientService *client);
