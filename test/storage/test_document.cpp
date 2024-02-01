#include "gtest/gtest.h"
#include "common.h"

extern "C" {
#include "private/storage/page_manager.h"
#include "public/util/common.h"
#include "private/document_db/document.h"
#include "public/util/memory.h"
#include "public/document_db/node_conditions.h"
#include "fs.h"
}

#define PAGE_SIZE 4096
#define SIGNATURE 0x12345678

TEST(document, create_multiple_nodes_delete_one) {
    char *data = "hello";
    CreateNodeRequest string_req = (CreateNodeRequest) {
            .parent = NULL_NODE_ID,
            .value = node_value_string_new(data)
    };
    Document *doc = document_new();
    remove(FILE_PATH);

    Result res = document_init(doc, FILE_PATH, PAGE_SIZE);
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
    auto *result = (NodesArray *) my_alloc(sizeof(NodesArray) + sizeof(Node) * MAX_NODES);
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

// adds nodes a -> b -> c -> d. Creates conditions and cals document_get_nodes_by_condition_sequence
TEST(document, get_node_by_condition_sequence) {
    Document *doc = document_new();
    remove(FILE_PATH);

    Result res = document_init(doc, FILE_PATH, PAGE_SIZE);
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

    CreateNodeRequest e_req = {
            .parent = c.id,
            .value = node_value_string_new("e (child of c)")
    };
    Node e = {0};
    res = document_add_node(doc, &e_req, &e);
    ASSERT_EQ(res.status, RES_OK);

    NodeConditionFunc funcs[] = {
        node_conditions_equals_str("a"),
        node_conditions_equals_str("b"),
        node_conditions_equals_str("c"),
        node_condition_all(),
    };
    NodeMatcherArray *matcherArr = node_matcher_array_new(funcs, sizeof(funcs) / sizeof(funcs[0]));

    int nodes_count = 0;
    res = document_count_nodes_by_condition_sequence(doc, matcherArr, &nodes_count);
    ASSERT_EQ(res.status, RES_OK);
    ASSERT_EQ(nodes_count, 2);

    auto *result = (NodesArray *) my_alloc(sizeof(NodesArray) + sizeof(Node) * nodes_count);

    result->count = nodes_count;
    res = document_get_nodes_by_condition_sequence(doc, matcherArr, result);
    ASSERT_EQ(res.status, RES_OK);
    ASSERT_EQ(result->count, 2);
    assert_node(&result->nodes[0], 0, 3, 0, 2, (NodeValue) {
        .type = STRING,
        .string_value = {
            .length = 1,
            .value = "d"
        }
    });
    assert_node(&result->nodes[1], 0, 4, 0, 2, (NodeValue) {
        .type = STRING,
        .string_value = {
            .length = static_cast<uint32_t>(strlen("e (child of c)")),
            .value = "e (child of c)"
        }
    });
}

TEST(document, many_child_nodes) {
    remove_file();
    Document *doc = document_new();
    Result res = document_init(doc, FILE_PATH, 512);
    ASSERT_EQ(res.status, RES_OK);

    char *files[] = {
            "ssl root 1705324315 inode/directory",
            "timezone root 1705324315 text/plain",
            "hosts root 1705324315 text/plain",
            "passwd root 1705324315 text/plain",
            "group root 1705324315 text/plain",
            "fstab root 1705324315 text/plain",
            "shadow root 1705324315 text/plain",
            "gshadow root 1705324315 text/plain",
    };

    CreateNodeRequest requests[9];
    requests[0] = {.parent = {.page_id = -1, .item_id = -1}, .value = node_value_string_new("/ root 1705324315 inode/directory")};

    for (int i = 0; i < 8; i++) {
        requests[i+1] = {.parent = {.page_id = 0, .item_id = 0}, .value = node_value_string_new(files[i])};;
    }

    for (auto & request : requests) {
        Node result;
        res = document_add_node(doc, &request, &result);
        ASSERT_EQ(res.status, RES_OK);
    }

    NodeMatcher *matcher = node_matcher_new(fs_new_attribute_condition("name", "/"));

    auto *nodeArr = (NodesArray*) my_alloc(sizeof(NodesArray) + sizeof(Node) * 1);
    nodeArr->count = 1;
    res = document_get_nodes_by_condition(doc, matcher, nodeArr);
    ASSERT_EQ(res.status, RES_OK);

    document_destroy(doc);
}