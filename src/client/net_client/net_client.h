#pragma once

#include "converters.h"

typedef struct ClientService ClientService;

ClientService *client_service_new(char *address);

void client_add_node(ClientService *self, CreateNodeRequest *request);

void client_update_node(ClientService *self, UpdateNodeRequest *request);

void client_delete_node(ClientService *self, DeleteNodeRequest *request);

void client_delete_all_nodes(ClientService *self);

void client_get_node(ClientService *self, node_id_t *request);

void client_get_all(ClientService *self);

void client_get_node_by_names_path(ClientService *self, char *path);

void client_get_node_by_filter(ClientService *self, Rpc__FilterChain *filters);

void run_main_loop(ClientService *self);