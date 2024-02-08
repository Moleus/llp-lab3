#include <stdbool.h>
#include <string.h>
#include <Block.h>
#include <stdlib.h>
#include "public/document_db/node_value_matcher.h"
#include "public/structures.h"
#include "fs.h"
#include "public/util/log.h"
#include "public/util/error.h"
#include "public/document_db/node_conditions.h"

NodeConditionFunc fs_new_attribute_condition(char *attribute, char *value) {
    return Block_copy( ^bool(Node node) {
        assert(node.value.type == STRING);
        assert(node.value.string_value.value != NULL);
        assert(strlen(node.value.string_value.value) > 0);
        FileInfo file_info = parse_file_info(node.value.string_value.value);
        FileInfoAttributes attr = file_info_attribute_from_string(attribute);
        bool result = file_info_attribute_matches(attr, value, file_info);
//        LOG_WARN("[fs_new_attribute_condition] attribute: %s. value: %s. result %d", attribute, value, result);
        return result;
    });
}

// converts FilterChain to NodeMatcherArray
// creates matcher based on selector field of each filter. It's either identifier (file name) or argument value
NodeMatcherArray *fs_new_node_matcher_array(const Rpc__FilterChain *filter_chain) {
    // TODO: полседний фильтр - получение всех детей
    NodeConditionFunc conditions[filter_chain->n_filters];
    for (int i = 0; i < filter_chain->n_filters; i++) {
        Rpc__Filter *filter = filter_chain->filters[i];
        if (filter->type == RPC__FILTER__TYPE__ALL) {
            conditions[i] = node_condition_all();
        } else {
            if (filter->field_name_case == RPC__FILTER__FIELD_NAME__NOT_SET) {
                ABORT_EXIT(SERVER_ERROR, "Filter field name not set")
            }
            conditions[i] = fs_new_attribute_condition(filter->name, filter->string_argument);
        }
    }

    return node_matcher_array_new(conditions, (int) filter_chain->n_filters);
}
