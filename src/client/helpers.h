#pragma once

#include "parser/types.h"
#include "converters.h"
#include "net_client.h"

Requests generate_requests();

Rpc__FilterChain* convertNodesToFilterChain(ParsedNode* nodes);

void make_request_based_on_query(Query *q, ClientService *client);

void add_nodes_sequence(Query *q, ClientService *client);

void get_nodes(Query *q, ClientService *client);
