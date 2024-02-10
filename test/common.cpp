#include "gtest/gtest.h"
#include <cstdlib>
#include "common.h"

void remove_file() {
    bool file_exists = access(FILE_PATH, F_OK) == 0;
    if (file_exists && remove(FILE_PATH) == -1) {
        printf("Failed to remove file %s\n", FILE_PATH);
        exit(1);
    }
}

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

NodesArray* setup_nodes(Document *doc) {
    CreateNodeRequest root_req = (CreateNodeRequest) {
            .parent = NULL_NODE_ID,
            .value = node_value_string_new("base root 1705324315 inode/directory")
    };

    Node root =  {0};
    Result res = document_add_node(doc, &root_req, &root);
    ABORT_IF_FAIL(res, "Failed to add root node");

    char *nodes[] = {
            "ssl root 1705324315 inode/directory",
            "hosts root 1705324315 text/plain",
            "passwd root 1705324315 text/plain"
    };

    char *ssl_children[] = {
            "cert.pem root 1705324315 text/plain",
            "private root 1705324315 inode/directory"
    };

    int nodes_count = sizeof(nodes) / sizeof(nodes[0]);
    int ssl_children_count = sizeof(ssl_children) / sizeof(ssl_children[0]);
    NodesArray *arr = (NodesArray *) my_alloc(sizeof(NodesArray) + sizeof(Node) * (nodes_count + ssl_children_count));
    for (int i = 0; i < nodes_count; i++) {
        CreateNodeRequest req = (CreateNodeRequest) {
                .parent = root.id,
                .value = node_value_string_new(nodes[i])
        };
        res = document_add_node(doc, &req, &(arr->nodes[i]));
        ABORT_IF_FAIL(res, "Failed to add child node");
    }

    for (int i = nodes_count; i < nodes_count + ssl_children_count; i++) {
        CreateNodeRequest req = (CreateNodeRequest) {
                .parent = arr->nodes[0].id,
                .value = node_value_string_new(ssl_children[i - nodes_count])
        };
        res = document_add_node(doc, &req, &(arr->nodes[i]));
        ABORT_IF_FAIL(res, "Failed to add child node");
    }

    return arr;
}
