
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

NodeConditionFunc fs_new_filename_condition(char *filename) {
    return Block_copy( ^bool(Node node) {
        assert(node.value.type == FILE_INFO);
        assert(node.value.file_info_value.name != NULL);
        assert(strlen(node.value.file_info_value.name) > 0);
        return strcmp(node.value.file_info_value.name, filename) == 0;
    });
}

NodeConditionFunc fs_new_owner_condition(char *owner) {
    return Block_copy( ^bool(Node node) {
        char *file = strdup(owner);
        return strcmp(node.value.file_info_value.owner, file) == 0;
    });
}


// converts FilterChain to NodeMatcherArray
NodeMatcherArray *fs_new_node_matcher_array(const Rpc__FilterChain *filter_chain) {
    NodeConditionFunc conditions[filter_chain->n_filters];
    for (int i = 0; i < filter_chain->n_filters; i++) {
        Rpc__Filter *filter = filter_chain->filters[i];
        if (filter->type != RPC__FILTER__TYPE__EQUAL) {
            LOG_ERR("unsupported filter type for fs: %d", filter->type);
            exit(EXIT_FAILURE);
        }
        if (filter->argument_case != RPC__FILTER__ARGUMENT_STRING_ARGUMENT) {
            LOG_ERR("unsupported filter argument case for fs: %d", filter->argument_case);
            exit(EXIT_FAILURE);
        }
        NodeConditionFunc condition = fs_new_filename_condition(filter->string_argument);
        conditions[i] = condition;
    }

    return node_matcher_array_new(conditions, (int) filter_chain->n_filters);
}