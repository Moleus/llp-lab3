#include "public/document_db/node_conditions.h"
#include <stdbool.h>
#include <string.h>
#include <Block.h>
#include <assert.h>
#include "public/structures.h"
#include "public/util/log.h"

NodeConditionFunc node_conditions_equals_str(char *expected_str) {
    return Block_copy( ^bool(Node node) {
        assert(node.value.type == STRING);
        assert(node.value.string_value.value != NULL);
        assert(strlen(node.value.string_value.value) > 0);
        char *value_str = node.value.string_value.value;
        LOG_WARN("[fs_new_filename_condition] expected_filename: %s. Actual %s", expected_str, value_str);
        return strcmp(value_str, expected_str) == 0;
    });
}
