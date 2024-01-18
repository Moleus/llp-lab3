
#include <stdbool.h>
#include <string.h>
#include <Block.h>
#include <stdlib.h>
#include "public/document_db/node_value_matcher.h"
#include "public/structures.h"
#include "fs.h"
#include "public/util/log.h"

NodeMatcherArray *fs_new_file_path_matchers(char *files[], int count) {
    NodeConditionFunc conditions[count];
    for (int i = 0; i < count; i++) {
        NodeConditionFunc condition = fs_new_filename_condition(files[i]);
        conditions[i] = condition;
    }

    return node_matcher_array_new(conditions, count);
}


NodeConditionFunc fs_new_filename_condition(char *expected_filename) {
    return Block_copy( ^bool(Node node) {
        assert(node.value.type == STRING);
        assert(node.value.string_value.value != NULL);
        assert(strlen(node.value.string_value.value) > 0);
        // extract first word from string_value (split by space)
        char *filename = strtok(node.value.string_value.value, " ");
        LOG_WARN("[fs_new_filename_condition] expected_filename: %s. Actual %s", expected_filename, filename);
        return strcmp(filename, expected_filename) == 0;
    });
}

// converts FilterChain to NodeMatcherArray
NodeMatcherArray *fs_new_node_matcher_array(const Rpc__FilterChain *filter_chain) {
    NodeConditionFunc conditions[filter_chain->n_filters];
    for (int i = 0; i < filter_chain->n_filters; i++) {
        Rpc__Filter *filter = filter_chain->filters[i];
        NodeConditionFunc condition = fs_new_filename_condition(filter->string_argument);
        conditions[i] = condition;
    }

    return node_matcher_array_new(conditions, (int) filter_chain->n_filters);
}