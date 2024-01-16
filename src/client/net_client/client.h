#pragma once

#include "converters.h"

typedef struct ClientService ClientService;


ClientService * client_service_new(char *address);

void client_add_node(ClientService *self, CreateNodeRequest *request);

void run_main_loop(ClientService *self);