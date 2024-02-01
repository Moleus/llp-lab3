#pragma once

void handle_get_nodes_by_filter_request(const Rpc__FilterChain *request, Rpc__Nodes *response);
void handle_delete_node_request(const Rpc__DeleteNodeRequest *request, Rpc__Node *response);

Document *server_init_document(const char* filepath, size_t page_size);

void handle_create_node_request(const Rpc__CreateNodeRequest *request, Rpc__Node *response);
