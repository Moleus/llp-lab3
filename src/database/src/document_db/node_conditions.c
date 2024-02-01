#include "public/document_db/node_conditions.h"
#include <stdbool.h>
#include <string.h>
#include <Block.h>
#include <assert.h>
#include "public/structures.h"

NodeConditionFunc node_conditions_equals_str(char *expected_str) {
    return Block_copy( ^bool(Node node) {
        assert(node.value.type == STRING);
        assert(node.value.string_value.value != NULL);
        assert(strlen(node.value.string_value.value) > 0);
        char *value_str = node.value.string_value.value;
//        LOG_WARN("[fs_new_filename_condition] expected_filename: %s. Actual %s", expected_str, value_str);
        return strcmp(value_str, expected_str) == 0;
    });
}

NodeConditionFunc node_conditions_parent_equals(node_id_t parent_id) {
    return Block_copy( ^bool(Node node) {
        return node.parent_id.page_id == parent_id.page_id && node.parent_id.item_id == parent_id.item_id;
    });
}

NodeConditionFunc node_condition_all(void) {
    return Block_copy(^bool(Node value) {
        return true;
    });
}

