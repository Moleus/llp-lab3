#pragma once

#include "public/util/result.h"
#include "public/storage/page_manager.h"
#include "node.h"
#include "node_value_matcher.h"
#include "public/structures.h"

typedef struct {
    Node *root_node;
    PageManager *page_manager;
    bool init_done;
} Document;

typedef struct {
    node_id_t parent;
    size_t count;
    NodeValue values[];
} CreateMultipleNodesRequest;

typedef struct {
    size_t count;
    Node node[];
} CreateMultipleNodesResult;

typedef struct {
    Node *node;
} GetAllChildrenRequest;

typedef struct {
    size_t count;
    Node nodes[];
} GetAllChildrenResult;

Document *document_new(void);

Result document_init(Document *self, const char *file_path, size_t page_size);

void document_destroy(Document *self);

Result document_add_node(Document *self, CreateNodeRequest *request, Node *result);

Result document_add_bulk_nodes(Document *self, CreateMultipleNodesRequest *request, CreateMultipleNodesResult *result);

Result document_delete_node(Document *self, DeleteNodeRequest *request, Node *result);

Result document_update_node(Document *self, UpdateNodeRequest *request, Node *result);

Result document_get_all_children(Document *self, GetAllChildrenRequest *request, GetAllChildrenResult *result);

Result document_get_node_by_condition(Document *self, NodeMatcher *matcher, Node *result);

Result document_get_node_by_condition_sequence(Document *self, NodeMatcherArray *matchers, Node *result);

double document_get_insertion_time_ms(void);

double document_get_deletion_time_ms(void);
