#pragma once

#include "node_value_matcher.h"

NodeMatcherArray *fs_new_file_path_matchers(char *files[], int count);

NodeConditionFunc node_conditions_equals_str(char *expected_str);