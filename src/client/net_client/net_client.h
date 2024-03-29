#pragma once

#include "converters.h"

extern Rpc__Nodes g_get_nodes_response;
extern Rpc__Node g_add_node_response;

typedef struct {
    protobuf_c_boolean is_done;
} Closure;

typedef struct ClientService ClientService;

ClientService *client_service_new(char *address);

void client_add_node(ClientService *self, CreateNodeRequest *request);

void client_update_node(ClientService *self, UpdateNodeRequest *request);

void client_delete_node(ClientService *self, DeleteNodeRequest *request);

void client_delete_node_by_filter(ClientService *self, Rpc__FilterChain *filters);

void client_delete_all_nodes(ClientService *self);

void client_get_node(ClientService *self, node_id_t *request);

void client_get_all(ClientService *self);

void client_get_node_by_names_path(ClientService *self, char *path);

void client_get_node_by_filter(ClientService *self, Rpc__FilterChain *filters);

void run_main_loop(ClientService *self);