#pragma once

#include "common.pb-c.h"

NodeMatcherArray *fs_new_file_path_matchers(char *files[], int count);

NodeConditionFunc fs_new_filename_condition(char *filename);

NodeMatcherArray *fs_new_node_matcher_array(const Rpc__FilterChain *filter_chain);

NodeConditionFunc fs_new_attribute_condition(char *attribute, char *value);