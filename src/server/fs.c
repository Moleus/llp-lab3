
#include <stdbool.h>
#include <string.h>
#include <Block.h>
#include "public/document_db/node_value_matcher.h"
#include "public/structures.h"
#include "fs.h"

NodeMatcherArray *fs_new_file_path_matchers(char *files[], int count) {
    NodeConditionFunc conditions[count];
    for (int i = 0; i < count; i++) {
        NodeConditionFunc condition = fs_new_filename_condition(files[i]);
        conditions[i] = condition;
    }

    return node_matcher_array_new(conditions, count);
}

NodeConditionFunc fs_new_filename_condition(char *filename) {
    return Block_copy( ^bool(Node node) {
        return strcmp(node.value.file_info_value.name, filename) == 0;
    });
}

NodeConditionFunc fs_new_owner_condition(char *owner) {
    return Block_copy( ^bool(Node node) {
        char *file = strdup(owner);
        return strcmp(node.value.file_info_value.owner, file) == 0;
    });
}