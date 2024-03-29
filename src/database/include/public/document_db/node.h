#pragma once

#include "public/util/common.h"
#include "../../../../rpc/converters.h"
#include "public/structures.h"
#include <stdint.h>


static const node_id_t ROOT_NODE_ID = {0, 0};
inline static bool is_root_node(node_id_t node_id) {
    return node_id.page_id == 0 && node_id.item_id == 0;
}

static const node_id_t NULL_NODE_ID = {-1, -1};
inline static bool is_null_node(node_id_t node_id) {
    return node_id.page_id == -1 && node_id.item_id == -1;
}

inline static bool node_id_eq(node_id_t node_id1, node_id_t node_id2) {
    return node_id1.page_id == node_id2.page_id && node_id1.item_id == node_id2.item_id;
}

Result node_new(size_t id, Node *parent, NodeValue value, Node *result);

Result node_add_child(Node *self, Node *child);

Result node_delete_child(Node *self, size_t child_id);

void node_destroy(Node *self);

Result node_get_value(Node *self, NodeValue *result);
