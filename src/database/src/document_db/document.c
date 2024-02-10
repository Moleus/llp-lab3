#include <assert.h>
#include "private/document_db/document.h"
#include "private/storage/page_manager.h"
#include "public/structures.h"
#include "public/util/memory.h"
#include <time.h>
#include <Block.h>

#define FILE_SIGNATURE 0x12345678

clock_t g_insert_start_time = 0;
clock_t g_insert_end_time = 0;

clock_t g_delete_start_time = 0;
clock_t g_delete_end_time = 0;

Result document_delete_second_step(Document *self, DeleteNodeRequest *request);

NodeConditionFunc node_condition_parent_id_eq(node_id_t node_id);

double document_get_insertion_time_ms(void) {
    double insertion_time = ((double) (g_insert_end_time - g_insert_start_time)) / CLOCKS_PER_SEC * 1000;
    return insertion_time;
}

double document_get_deletion_time_ms(void) {
    double deletion_time = ((double) (g_delete_end_time - g_delete_start_time)) / CLOCKS_PER_SEC * 1000;
    return deletion_time;
}

node_id_t item_to_node_id(item_index_t item_id) {
    return (node_id_t) {.page_id = item_id.page_id.id, .item_id = item_id.item_id};
}

Node *item_to_node(Item item) {
    assert(item.payload.size > 0);
    Node *node = item.payload.data;
    node->id = item_to_node_id(item.id);
    return node;
}

Document *document_new() {
    Document *document = malloc(sizeof(Document));
    ASSERT_NOT_NULL(document, FAILED_TO_ALLOCATE_MEMORY)
    document->page_manager = page_manager_new();
    document->root_node = NULL;
    document->init_done = false;
    return document;
}

void set_root_node(Document *self) {
    self->root_node = my_alloc(sizeof(Node));
    NodeMatcher *matcher = node_matcher_new(node_condition_parent_id_eq(ROOT_NODE_ID));
    Result res = document_take_first_node_by_condition(self, matcher, self->root_node);
    if (res.status != RES_OK) {
        LOG_WARN("Failed to get root node: %s", res.message);
    }
    node_matcher_destroy(matcher);
}

Result document_init(Document *self, const char *file_path, size_t page_size) {
    self->init_done = true;
    Result init = page_manager_init(self->page_manager, file_path, page_size, FILE_SIGNATURE);
    RETURN_IF_FAIL(init, "failed to init page manager")
    set_root_node(self);
    return init;
}

void document_destroy(Document *self) {
    ASSERT_ARG_NOT_NULL(self)
    assert(self->init_done);

    page_manager_destroy(self->page_manager);
    free(self);
}

// --- ADD NODE ---
// persists new node and assigns id to it
Result document_persist_new_node(Document *self, Node *node) {
    ASSERT_ARG_NOT_NULL(self)
    ASSERT_ARG_NOT_NULL(node)

    g_insert_start_time = clock();

    ItemPayload itemPayload = {
            .data = node,
            .size = sizeof(Node)
    };

    Page *page = NULL;
    Result res = page_manager_get_page_for_data(self->page_manager, &page);
    RETURN_IF_FAIL(res, "failed to persist new data")

    ItemAddResult item_result = {0};
    res = page_manager_put_item(self->page_manager, page, itemPayload, &item_result);
    RETURN_IF_FAIL(res, "failed to persist new data")

    node->id = item_to_node_id(item_result.metadata.item_id);
    page_manager_free_pages(self->page_manager);

    g_insert_end_time = clock();

    return OK;
}

Result document_add_root_node(Document *self, Node *root) {
    ASSERT_ARG_NOT_NULL(self)
    ASSERT_ARG_NOT_NULL(root)
    // check that we have only 1 root node in file
    // if we have 1 root node, then return error
    // if we have 0 root nodes, then create new root node

    LOG_DEBUG("[add_root_node] Adding root node with type %d", root->value.type);

    Item item;
    ItemIterator *items_it = page_manager_get_items(self->page_manager, &item);
    while (item_iterator_has_next(items_it)) {
        Result get_item_res = item_iterator_next(items_it, &item);
        RETURN_IF_FAIL(get_item_res, "failed to add root node")
        Node *node = item_to_node(item);
        if (is_root_node(node->id)) {
            return ERROR("Root node already exists in document tree");
        }
    }
    item_iterator_destroy(items_it);

    root->id = (node_id_t) {.page_id = 0, .item_id = 0};
    self->root_node = root;

    return document_persist_new_node(self, root);
}

Result document_add_child_node(Document *self, Node *current_node) {
    Item item;

    LOG_DEBUG("Document - Adding child node with type %d to parent: (%d:%d)", current_node->value.type,
              current_node->parent_id.page_id, current_node->parent_id.item_id);
    ItemIterator *items_it = page_manager_get_items(self->page_manager, &item);
    while (item_iterator_has_next(items_it)) {
        Result get_item_res = item_iterator_next(items_it, &item);
        ABORT_IF_FAIL(get_item_res, "Failed to get next item in iterator")
        Node *tmp_node = item_to_node(item);
        if (node_id_eq(tmp_node->id, current_node->parent_id)) {
            // found parent node. Ok. Adding new
            item_iterator_destroy(items_it);
            return document_persist_new_node(self, current_node);
        }
    }
    item_iterator_destroy(items_it);
    // didn't find parent node
    return ERROR("Parent node doesn't exist in document tree");
}

Result document_add_node(Document *self, CreateNodeRequest *request, Node *result) {
    ASSERT_ARG_NOT_NULL(self)
    ASSERT_ARG_NOT_NULL(request)
    ASSERT_ARG_NOT_NULL(result)
    assert(self->init_done);

    result->value = request->value;
    result->parent_id = request->parent;

    LOG_DEBUG("[document_add_node] Adding node of type %d to document", result->value.type);

    if (is_null_node(request->parent)) {
        return document_add_root_node(self, result);
    } else {
        return document_add_child_node(self, result);
    }
}

//Result document_get_nodes_by_condition(Document *self, NodeMatcher *matcher, Node *result) {
Result document_delete_nodes_by_condition(Document *self, NodeMatcher *matcher, int *deleted_count) {
    ASSERT_ARG_NOT_NULL(self)
    ASSERT_ARG_NOT_NULL(matcher)
    ASSERT_ARG_NOT_NULL(deleted_count)
    assert(self->init_done);

    NodesArray *all_children_result = my_alloc(sizeof(NodesArray) + sizeof(Node) * 1000);
    Result res = document_get_all_nodes(self, all_children_result);
    RETURN_IF_FAIL(res, "failed to get all nodes")

    for (size_t i = 0; i < all_children_result->count; i++) {
        Node *node = &all_children_result->nodes[i];
        if (node_condition_matches(matcher, *node)) {
            res = delete_node_with_all_descendants(self, node->id, deleted_count);
            RETURN_IF_FAIL(res, "failed to delete node")
            (*deleted_count)++;
        }
    }
    free(all_children_result);
    return OK;
}

Result delete_node_with_all_descendants(Document *self, node_id_t node_id, int *deleted_count) {
    int children_count = 0;
    Result res = document_count_children(self, node_id, &children_count);
    ABORT_IF_FAIL(res, "Failed to count children of node");
    if (children_count > 0) {
        NodesArray *children = nodes_array_new(children_count);
        NodeMatcher *matcher = node_matcher_new(node_condition_parent_id_eq(node_id));
        res = document_get_nodes_by_condition(self, matcher, children);
        ABORT_IF_FAIL(res, "Failed to get children of node");

        for (size_t i = 0; i < children->count; i++) {
            Node *child = &children->nodes[i];
            res = delete_node_with_all_descendants(self, child->id, deleted_count);
            ABORT_IF_FAIL(res, "Failed to delete node");
        }
    }

    // has no children
    (*deleted_count)++;
    Node tmp;
    res = document_delete_node(self, &(DeleteNodeRequest) {.node_id = node_id}, &tmp);
    ABORT_IF_FAIL(res, "Trying to delete node with not children");
    return res;
}

// --- DELETE NODE ---
Result document_delete_node(Document *self, DeleteNodeRequest *request, Node *result) {
    ASSERT_ARG_NOT_NULL(self)
    ASSERT_ARG_NOT_NULL(request)
    assert(self->init_done);

    LOG_DEBUG("Document - Deleting node (%d:%d)", request->node_id.page_id, request->node_id.item_id);
    // if node contains children - raise error
    Item item = {0};
    ItemIterator *items_it = page_manager_get_items(self->page_manager, &item);
    while (item_iterator_has_next(items_it)) {
        Result get_item_res = item_iterator_next(items_it, &item);
        RETURN_IF_FAIL(get_item_res, "failed to delete node")
        Node *tmp_node = item_to_node(item);

        if (node_id_eq(tmp_node->parent_id, request->node_id)) {
            // We are trying to delete node with children
            item_iterator_destroy(items_it);
            LOG_WARN("Can't delete node with childen: %d/%d; child value: %s", request->node_id.page_id, request->node_id.item_id, tmp_node->value.string_value.value);
            ABORT_EXIT(INTERNAL_LIB_ERROR, "Can't delete node with children");
        }
    }
    item_iterator_destroy(items_it);
    return document_delete_second_step(self, request);
}

Result document_delete_second_step(Document *self, DeleteNodeRequest *request) {
    g_delete_start_time = clock();

    ItemIterator *items_it = NULL;
    Item item = {0};
    items_it = page_manager_get_items(self->page_manager, &item);
    // find our node
    while (item_iterator_has_next(items_it)) {
        Result get_item_res = item_iterator_next(items_it, &item);
        RETURN_IF_FAIL(get_item_res, "failed to delete node")
        item_index_t item_id = item.id;
        if (item_id.page_id.id == request->node_id.page_id && item_id.item_id == request->node_id.item_id) {
            // We found our node
            Page *page = items_it->page_iterator->current_page;
            Result res = page_manager_delete_item(self->page_manager, page, &item);
            ABORT_IF_FAIL(res, "failed to delete node");
            item_iterator_destroy(items_it);
            g_delete_end_time = clock();
            return OK;
        }
    }
    item_iterator_destroy(items_it);

    // didn't find node
    return ERROR("Node doesn't exist in document tree");
}

NodeConditionFunc node_condition_parent_id_eq(node_id_t node_id) {
    return Block_copy(^bool(Node value) {
        return node_id_eq(value.parent_id, node_id);
    });
}

// --- UPDATE NODE ---
Result document_update_node(Document *self, UpdateNodeRequest *request, Node *result) {
    ASSERT_ARG_NOT_NULL(self)
    ASSERT_ARG_NOT_NULL(request)
    assert(self->init_done);
    // delete + add

    DeleteNodeRequest delete_request = {
            .node_id = request->node_id
    };
    CreateNodeRequest create_request = {
            .parent = request->parent_id,
            .value = request->new_value
    };
    Result res = document_delete_node(self, &delete_request, result);
    RETURN_IF_FAIL(res, "failed to delete node while updating")
    res = document_add_node(self, &create_request, result);
    RETURN_IF_FAIL(res, "failed to add node while updating")
    return OK;
}

// --- ADD MULTIPLE NODES ---
Result document_add_bulk_nodes(Document *self, CreateMultipleNodesRequest *request, CreateMultipleNodesResult *result) {
    ASSERT_ARG_NOT_NULL(self)
    ASSERT_ARG_NOT_NULL(request)
    ASSERT_ARG_NOT_NULL(result)
    assert(self->init_done);

    if (request->count == 0) {
        return ERROR("Bulk add, empty request");
    }

    Item item;
    ItemIterator *items_it = page_manager_get_items(self->page_manager, &item);
    while (item_iterator_has_next(items_it)) {
        Result get_item_res = item_iterator_next(items_it, &item);
        RETURN_IF_FAIL(get_item_res, "failed to bulk add node")
        Node *tmp_node = item_to_node(item);
        if (node_id_eq(tmp_node->id, request->parent)) {
            // found parent node. Ok. Adding new
            result->count = request->count;
            for (size_t i = 0; i < request->count; i++) {
                Node *node = &result->node[i];
                node->value = request->values[i];
                node->parent_id = request->parent;
                Result res = document_persist_new_node(self, node);
                item_iterator_destroy(items_it);
                RETURN_IF_FAIL(res, "failed to bulk add node")
            }
            item_iterator_destroy(items_it);
            return OK;
        }
    }
    item_iterator_destroy(items_it);
    // didn't find parent node
    return ERROR("Parent node doesn't exist in document tree");
}

Result document_get_all_nodes(Document *self, NodesArray *result) {
    GetAllChildrenRequest request = {
            .parent = NULL_NODE_ID
    };
    return document_get_all_children(self, &request, result);
}

// --- GET ALL CHILDREN ---
Result document_get_all_children(Document *self, GetAllChildrenRequest *request, NodesArray *result) {
    ASSERT_ARG_NOT_NULL(self)
    ASSERT_ARG_NOT_NULL(request)
    ASSERT_ARG_NOT_NULL(result)
    assert(self->init_done);

    Item item;
    ItemIterator *items_it = page_manager_get_items(self->page_manager, &item);

    size_t count = 0;
    while (item_iterator_has_next(items_it)) {
        Result get_item_res = item_iterator_next(items_it, &item);
        RETURN_IF_FAIL(get_item_res, "failed to bulk add node")
        Node *tmp_node = item_to_node(item);
        if (node_id_eq(tmp_node->parent_id, request->parent)) {
            count++;
        }
    }
    item_iterator_destroy(items_it);
    result->count = count;
    if (count == 0) {
        return OK;
    }
    size_t i = 0;
    items_it = page_manager_get_items(self->page_manager, &item);
    while (item_iterator_has_next(items_it)) {
        Result get_item_res = item_iterator_next(items_it, &item);
        RETURN_IF_FAIL(get_item_res, "failed to bulk add node")
        Node *tmp_node = item_to_node(item);
        if (node_id_eq(tmp_node->parent_id, request->parent)) {
            result->nodes[i++] = *tmp_node;
        }
    }
    item_iterator_destroy(items_it);
    // didn't find parent node
    return OK;
}

Result document_count_children(Document *self, node_id_t parent_id, int *children_result) {
    NodeMatcher *matcher = node_matcher_new(node_condition_parent_id_eq(parent_id));
    return document_count_nodes_by_condition(self, matcher, children_result);
}

Result document_count_nodes_by_condition(Document *self, NodeMatcher *matcher, int *count) {
    ASSERT_ARG_NOT_NULL(self)
    ASSERT_ARG_NOT_NULL(matcher)
    ASSERT_ARG_NOT_NULL(count)
    assert(self->init_done);

    Item item;
    ItemIterator *items_it = page_manager_get_items(self->page_manager, &item);
    while (item_iterator_has_next(items_it)) {
        Result get_item_res = item_iterator_next(items_it, &item);
        ABORT_IF_FAIL(get_item_res, "failed to bulk add node")
        Node *tmp_node = item_to_node(item);
        if (node_condition_matches(matcher, *tmp_node)) {
            (*count)++;
        }
    }
    item_iterator_destroy(items_it);
    return OK;
}

Result document_take_first_node_by_condition(Document *self, NodeMatcher *matcher, Node *result) {
    ASSERT_ARG_NOT_NULL(self)
    ASSERT_ARG_NOT_NULL(matcher)
    ASSERT_ARG_NOT_NULL(result)
    assert(self->init_done);

    Item item;
    ItemIterator *items_it = page_manager_get_items(self->page_manager, &item);
    while (item_iterator_has_next(items_it)) {
        Result get_item_res = item_iterator_next(items_it, &item);
        ABORT_IF_FAIL(get_item_res, "failed to bulk add node")
        Node *tmp_node = item_to_node(item);
        if (node_condition_matches(matcher, *tmp_node)) {
            *result = *tmp_node;
            item_iterator_destroy(items_it);
            return OK;
        }
    }
    item_iterator_destroy(items_it);
    return ERROR("No nodes found by condition");
}

Result document_get_nodes_by_condition(Document *self, NodeMatcher *matcher, NodesArray *result) {
    ASSERT_ARG_NOT_NULL(self)
    ASSERT_ARG_NOT_NULL(matcher)
    ASSERT_ARG_NOT_NULL(result)
    assert(self->init_done);
    assert(result->count > 0);

    int i = 0;
    Item item;
    ItemIterator *items_it = page_manager_get_items(self->page_manager, &item);
    while (item_iterator_has_next(items_it)) {
        Result get_item_res = item_iterator_next(items_it, &item);
        ABORT_IF_FAIL(get_item_res, "failed to get node from iterator")
        Node *tmp_node = item_to_node(item);
        if (node_condition_matches(matcher, *tmp_node)) {
            result->nodes[i++] = *tmp_node;
        }
    }
    item_iterator_destroy(items_it);

    assert(result->count == i);
    if (i == 0) {
        return ERROR("No nodes found by condition");
    }
    return OK;
}

Result document_count_nodes_by_condition_sequence(Document *self, NodeMatcherArray *matchers, int *count_result) {
    ASSERT_ARG_NOT_NULL(self)
    ASSERT_ARG_NOT_NULL(matchers)
    ASSERT_ARG_NOT_NULL(count_result)
    assert(self->init_done);

    Node *current_node = malloc(sizeof(Node));
    ASSERT_NOT_NULL(current_node, FAILED_TO_ALLOCATE_MEMORY)
    *current_node = *self->root_node;

    node_id_t parent_id = NULL_NODE_ID;

    int matched_nodes_count = 0;

    for (size_t i = 0; i < matchers->matchers_count; i++) {
        ASSERT_NOT_NULL(matchers->matchers[i], INTERNAL_LIB_ERROR)
        NodeMatcher *matcher = matchers->matchers[i];
        node_add_condition(matcher, node_condition_parent_id_eq(parent_id));

        matched_nodes_count = 0;
        Result res = document_count_nodes_by_condition(self, matcher, &matched_nodes_count);
        ABORT_IF_FAIL(res, "Failed to count nodes by condition");

        if (matched_nodes_count == 0) {
            free(current_node);
            *count_result = 0;
            node_remove_last_condition(matcher);
            return OK;
        }

        NodesArray *nodes = my_alloc(sizeof(NodesArray) + sizeof(Node) * matched_nodes_count);
        nodes->count = matched_nodes_count;

        res = document_get_nodes_by_condition(self, matcher, nodes);
        ABORT_IF_FAIL(res, "Failed to get node by condition");

        *current_node = nodes->nodes[0];
        free(nodes);

        parent_id = current_node->id;
    }
    *count_result = matched_nodes_count;
    free(current_node);
    return OK;
}

// takes an array of matchers. Applies first on all nodes, then when condition matches, applies second matcher on children of first node, etc
// until we satisfy the last matcher or none of the matchers match
// After the node matches the condition, we add new condition that parent is the previous node id
// when we match we delete the last condition
Result document_get_nodes_by_condition_sequence(Document *self, NodeMatcherArray *matchers, NodesArray *result) {
    ASSERT_ARG_NOT_NULL(self)
    ASSERT_ARG_NOT_NULL(result)
    assert(self->init_done);
    assert(result->count > 0);

    Node *current_node = malloc(sizeof(Node));
    ASSERT_NOT_NULL(current_node, FAILED_TO_ALLOCATE_MEMORY)
    *current_node = *self->root_node;

    node_id_t parent_id = NULL_NODE_ID;

    NodesArray* nodes_array_buffer = NULL;

    for (size_t i = 0; i < matchers->matchers_count; i++) {
        ASSERT_NOT_NULL(matchers->matchers[i], INTERNAL_LIB_ERROR)
        NodeMatcher *matcher = matchers->matchers[i];
        node_add_condition(matcher, node_condition_parent_id_eq(parent_id));

        int matched_nodes_count = 0;
        Result res = document_count_nodes_by_condition(self, matcher, &matched_nodes_count);
        ABORT_IF_FAIL(res, "Failed to count nodes by condition");

        if (matched_nodes_count == 0) {
            free(current_node);
            node_remove_last_condition(matcher);
            return ERROR("No nodes found by condition");
        }

        if (i == matchers->matchers_count - 1) {
            // last matcher
            assert(result->count == matched_nodes_count);
            nodes_array_buffer = result;
        } else {
            nodes_array_buffer = my_alloc(sizeof(NodesArray) + sizeof(Node) * matched_nodes_count);
        }

        nodes_array_buffer->count = matched_nodes_count;
        res = document_get_nodes_by_condition(self, matcher, nodes_array_buffer);
        if (res.status != RES_OK) {
            LOG_DEBUG("Failed to get node by condition: %s", res.message);
            free(current_node);
            node_remove_last_condition(matcher);
            return res;
        }
        node_remove_last_condition(matcher);

        if (i != matchers->matchers_count - 1) {
            // free intermediate buffer
            *current_node = nodes_array_buffer->nodes[0];
            free(nodes_array_buffer);
        }

        parent_id = current_node->id;
    }

    free(current_node);
    return OK;
}

