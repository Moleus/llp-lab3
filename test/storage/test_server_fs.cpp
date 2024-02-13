#include "gtest/gtest.h"
#include "common.h"

extern "C" {
#include "public/util/memory.h"
#include "public/document_db/node_conditions.h"
#include "fs.h"
}

/*
 * NodeMatcherArray *fs_new_node_matcher_array(const Rpc__FilterChain *filter_chain) {
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
 */

TEST(server, fs_new_node_matcher_array) {
    Rpc__Filter root_filter = RPC__FILTER__INIT;
    root_filter.type = RPC__FILTER__TYPE__EQUAL;
    root_filter.argument_case = RPC__FILTER__ARGUMENT_STRING_ARGUMENT;
    root_filter.string_argument = "root";
    root_filter.name = "name";
    root_filter.field_name_case = RPC__FILTER__FIELD_NAME_NAME;

    Rpc__Filter filter = RPC__FILTER__INIT;
    filter.type = RPC__FILTER__TYPE__EQUAL;
    filter.argument_case = RPC__FILTER__ARGUMENT_STRING_ARGUMENT;
    filter.string_argument = "passwd";
    filter.name = "name";
    filter.field_name_case = RPC__FILTER__FIELD_NAME_NAME;

    Rpc__FilterChain chain = RPC__FILTER_CHAIN__INIT;
    chain.n_filters = 2;
    chain.filters = static_cast<Rpc__Filter **>(my_alloc(sizeof(Rpc__Filter *) * 2));
    chain.filters[0] = &root_filter;
    chain.filters[1] = &filter;

    NodeMatcherArray *result = fs_new_node_matcher_array(&chain);

    ASSERT_TRUE(node_condition_matches(result->matchers[0], (Node) {.value = node_value_string_new("root")}));
    ASSERT_TRUE(node_condition_matches(result->matchers[1], (Node) {.value = node_value_string_new("passwd")}));
}

TEST(server, fs_matcher_array_with_field) {
    Rpc__Filter root_filter = RPC__FILTER__INIT;
    root_filter.type = RPC__FILTER__TYPE__EQUAL;
    root_filter.argument_case = RPC__FILTER__ARGUMENT_STRING_ARGUMENT;
    root_filter.string_argument = "file-name";
    root_filter.name = "name";
    root_filter.field_name_case = RPC__FILTER__FIELD_NAME_NAME;

    Rpc__Filter filter = RPC__FILTER__INIT;
    filter.type = RPC__FILTER__TYPE__EQUAL;
    filter.argument_case = RPC__FILTER__ARGUMENT_STRING_ARGUMENT;
    filter.string_argument = "a";
    filter.name = "owner";
    filter.field_name_case = RPC__FILTER__FIELD_NAME_NAME;

    Rpc__FilterChain chain = RPC__FILTER_CHAIN__INIT;
    chain.n_filters = 2;
    chain.filters = static_cast<Rpc__Filter **>(my_alloc(sizeof(Rpc__Filter *) * 2));
    chain.filters[0] = &root_filter;
    chain.filters[1] = &filter;

    NodeMatcherArray *result = fs_new_node_matcher_array(&chain);

    ASSERT_TRUE(node_condition_matches(result->matchers[0], (Node) {.value = node_value_string_new("file-name root 1705324315 inode.directory")}));
    ASSERT_TRUE(node_condition_matches(result->matchers[1], (Node) {.value = node_value_string_new("x a 1705324315 inode.directory")}));
}
