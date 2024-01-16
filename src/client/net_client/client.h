#pragma once

#include "common.h"

typedef struct ClientService ClientService;


ClientService * client_service_new(char *address);

void client_add_fileNode(ClientService *self, CreateFileNodeRequest *request);

void run_main_loop(ClientService *self);