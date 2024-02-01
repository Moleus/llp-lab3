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

