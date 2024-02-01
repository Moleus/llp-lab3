#include "gtest/gtest.h"

extern "C" {
#include "private/storage/page_manager.h"
#include "public/util/common.h"
#include "private/document_db/document.h"
#include "public/util/memory.h"
#include "public/document_db/node_conditions.h"
}

#define PAGE_SIZE 80
#define SIGNATURE 0x12345678

#define FILE_PATH "/tmp/test.llp"

void assert_node(Node *node, int page_id, int item_id, int parent_page_id, int parent_item_id, NodeValue value) {
    ASSERT_EQ(node->id.page_id, page_id);
    ASSERT_EQ(node->id.item_id, item_id);
    ASSERT_EQ(node->parent_id.page_id, parent_page_id);
    ASSERT_EQ(node->parent_id.item_id, parent_item_id);
    ASSERT_EQ(node->value.type, value.type);
    if (value.type == STRING) {
        ASSERT_EQ(node->value.string_value.length, value.string_value.length);
        for (int i = 0; i < value.string_value.length; i++) {
            ASSERT_EQ(node->value.string_value.value[i], value.string_value.value[i]);
        }
    } else if (value.type == DOUBLE) {
        ASSERT_EQ(node->value.double_value, value.double_value);
    }
}

TEST(document, create_multiple_nodes_delete_one) {
    char *data = "hello";
    CreateNodeRequest string_req = (CreateNodeRequest) {
            .parent = NULL_NODE_ID,
            .value = node_value_string_new(data)
    };
    Document *doc = document_new();
    remove(FILE_PATH);

    Result res = document_init(doc, FILE_PATH, 512);
    ASSERT_EQ(res.status, RES_OK);

    Node string_node = {0};
    Node double_node = {0};
    res = document_add_node(doc, &string_req, &string_node);
    ASSERT_EQ(res.status, RES_OK);
    assert_node(&string_node, 0, 0, -1, -1, (NodeValue) {
            .type = STRING,
            .string_value = {
                    .length = 5,
                    .value = "hello"
            }
    });

    // double value request
    CreateNodeRequest double_req = {
            .parent = string_node.id,
            .value = (NodeValue) {
                    .type = DOUBLE,
                    .double_value = 3.14
            },
    };
    res = document_add_node(doc, &double_req, &double_node);
    ASSERT_EQ(res.status, RES_OK);

    // remove 1 node
    LOG_INFO("string node id: %d, data: %.*s", string_node.id, string_node.value.string_value.length,
             string_node.value.string_value.value);

    DeleteNodeRequest delete_req = {
            .node_id = double_node.id,
    };
    document_delete_node(doc, &delete_req, &double_node);
    LOG_INFO("double node id: %d, data: %f", double_node.id, double_node.value.double_value);

    document_destroy(doc);

    Document *new_doc = document_new();
    res = document_init(new_doc, FILE_PATH, 512);
    ASSERT_EQ(res.status, RES_OK);

    // get all nodes
    int MAX_NODES = 10;
    auto *result = (GetAllChildrenResult *) my_alloc(sizeof(GetAllChildrenResult) + sizeof(Node) * MAX_NODES);
    res = document_get_all_nodes(new_doc, result);
    ASSERT_EQ(res.status, RES_OK);

    Node actual_root_node = result->nodes[0];
    assert_node(&actual_root_node, ROOT_NODE_ID.page_id, ROOT_NODE_ID.item_id, NULL_NODE_ID.page_id,
                NULL_NODE_ID.item_id, (NodeValue) {
                    .type = STRING,
                    .string_value = {
                            .length = 5,
                            .value = "hello"
                    }
            });

    document_destroy(new_doc);
}

// add 3 nodes a -> b -> c
TEST(document, add_chain_of_nodes) {
    Document *doc = document_new();
    remove(FILE_PATH);

    Result res = document_init(doc, FILE_PATH, 512);
    ASSERT_EQ(res.status, RES_OK);

    // add nodes
    CreateNodeRequest a_req = {
            .parent = NULL_NODE_ID,
            .value = node_value_string_new("a")
    };
    Node a = {0};
    res = document_add_node(doc, &a_req, &a);
    ASSERT_EQ(res.status, RES_OK);

    CreateNodeRequest b_req = {
            .parent = a.id,
            .value = node_value_string_new("b")
    };
    Node b = {0};
    res = document_add_node(doc, &b_req, &b);
    ASSERT_EQ(res.status, RES_OK);

    CreateNodeRequest c_req = {
            .parent = b.id,
            .value = node_value_string_new("c")
    };
    Node c = {0};
    res = document_add_node(doc, &c_req, &c);
    ASSERT_EQ(res.status, RES_OK);

    // get all nodes
    int MAX_NODES = 10;
    auto *result = (GetAllChildrenResult *) my_alloc(sizeof(GetAllChildrenResult) + sizeof(Node) * MAX_NODES);
    GetAllChildrenRequest req = {
            .parent = NULL_NODE_ID
    };
    res = document_get_all_children(doc, &req, result);
    ASSERT_EQ(res.status, RES_OK);
    ASSERT_EQ(result->count, 1);
    assert_node(&result->nodes[0], 0, 0, -1, -1, (NodeValue) {
        .type = STRING,
        .string_value = {
            .length = 1,
            .value = "a"
        }
    });

    GetAllChildrenRequest req2 = {
            .parent = a.id
    };
    res = document_get_all_children(doc, &req2, result);
    ASSERT_EQ(res.status, RES_OK);
    ASSERT_EQ(result->count, 1);
    assert_node(&result->nodes[0], 0, 1, 0, 0, (NodeValue) {
        .type = STRING,
        .string_value = {
            .length = 1,
            .value = "b"
        }
    });
}

// adds nodes a -> b -> c -> d. Creates conditions and cals document_get_node_by_condition_sequence
TEST(document, get_node_by_condition_sequence) {
    Document *doc = document_new();
    remove(FILE_PATH);

    Result res = document_init(doc, FILE_PATH, 512);
    ASSERT_EQ(res.status, RES_OK);

    // add nodes
    CreateNodeRequest a_req = {
            .parent = NULL_NODE_ID,
            .value = node_value_string_new("a")
    };
    Node a = {0};
    res = document_add_node(doc, &a_req, &a);
    ASSERT_EQ(res.status, RES_OK);

    CreateNodeRequest b_req = {
            .parent = a.id,
            .value = node_value_string_new("b")
    };
    Node b = {0};
    res = document_add_node(doc, &b_req, &b);
    ASSERT_EQ(res.status, RES_OK);

    CreateNodeRequest c_req = {
            .parent = b.id,
            .value = node_value_string_new("c")
    };
    Node c = {0};
    res = document_add_node(doc, &c_req, &c);
    ASSERT_EQ(res.status, RES_OK);

    CreateNodeRequest d_req = {
            .parent = c.id,
            .value = node_value_string_new("d")
    };
    Node d = {0};
    res = document_add_node(doc, &d_req, &d);
    ASSERT_EQ(res.status, RES_OK);

    // get all nodes
    int MAX_NODES = 10;
    auto *result = (GetAllChildrenResult *) my_alloc(sizeof(GetAllChildrenResult) + sizeof(Node) * MAX_NODES);

    NodeConditionFunc funcs[] = {
        node_conditions_equals_str("a"),
        node_conditions_equals_str("b"),
        node_conditions_equals_str("c"),
    };
    NodeMatcherArray *matcherArr = node_matcher_array_new(funcs, 3);
    res = document_get_node_by_condition_sequence(doc, matcherArr, &result->nodes[0]);
    ASSERT_EQ(res.status, RES_OK);
}