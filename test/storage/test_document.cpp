#include "gtest/gtest.h"

extern "C" {
#include "private/storage/page_manager.h"
#include "public/util/common.h"
#include "private/document_db/document.h"
#include "public/util/memory.h"
}

#define PAGE_SIZE 80
#define SIGNATURE 0x12345678

#define FILE_PATH "/tmp/test.llp"

TEST(document, create_multiple_nodes_delete_one) {
    char *data = "hello";
    CreateNodeRequest *string_req = (CreateNodeRequest*) my_alloc(sizeof(CreateNodeRequest) + strlen(data) + 1);
    *string_req = (CreateNodeRequest) {
            .parent = NULL_NODE_ID,
            .value = *node_value_string_new(data)
    };
    strcpy(string_req->value.string_value.value, data);
    Document *doc = document_new();
    remove(FILE_PATH);

    Result res = document_init(doc, FILE_PATH, 512);
    ASSERT_EQ(res.status, RES_OK);

    Node string_node = {0};
    Node double_node = {0};
    res = document_add_node(doc, string_req, &string_node);
    ASSERT_EQ(res.status, RES_OK);
    ASSERT_EQ(string_node.id.item_id, 0);
    ASSERT_EQ(string_node.id.page_id, 0);
    ASSERT_EQ(string_node.parent_id.item_id, -1);
    ASSERT_EQ(string_node.parent_id.page_id, -1);
    ASSERT_EQ(string_node.value.string_value.length, 5);
    ASSERT_EQ(string_node.value.string_value.value[0], 'h');
    ASSERT_EQ(string_node.value.string_value.value[1], 'e');
    ASSERT_EQ(string_node.value.string_value.value[2], 'l');
    ASSERT_EQ(string_node.value.string_value.value[3], 'l');
    ASSERT_EQ(string_node.value.string_value.value[4], 'o');

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

    Document * new_doc = document_new();
    res = document_init(new_doc, FILE_PATH, 512);
    ASSERT_EQ(res.status, RES_OK);

    // get all nodes
    int MAX_NODES = 10;
    GetAllChildrenResult *result = (GetAllChildrenResult*) my_alloc(sizeof(GetAllChildrenResult) + sizeof(Node) * MAX_NODES);
    res = document_get_all_nodes(new_doc, result);
    ASSERT_EQ(res.status, RES_OK);

    Node actual_root_node = result->nodes[0];
    ASSERT_EQ(actual_root_node.id.page_id, ROOT_NODE_ID.page_id);
    ASSERT_EQ(actual_root_node.id.item_id, ROOT_NODE_ID.item_id);
    ASSERT_EQ(actual_root_node.parent_id.page_id, NULL_NODE_ID.page_id);
    ASSERT_EQ(actual_root_node.parent_id.item_id, NULL_NODE_ID.item_id);
    ASSERT_EQ(actual_root_node.value.type, STRING);
    ASSERT_EQ(actual_root_node.value.string_value.length, 5);
    ASSERT_EQ(actual_root_node.value.string_value.value[0], 'h');
    ASSERT_EQ(actual_root_node.value.string_value.value[1], 'e');
    ASSERT_NE(actual_root_node.value.string_value.value, string_node.value.string_value.value);


    document_destroy(new_doc);
}