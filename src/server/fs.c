
#include <stdbool.h>
#include <string.h>
#include "public/document_db/node_value_matcher.h"
#include "public/structures.h"

NodeConditionFunc node_value_new_filename_condition(char *filename) {
    NodeConditionFunc condition = ^bool(Node node) {
        return strcmp(node.value.file_info_value.name, filename) == 0;
    };
    return condition;
}