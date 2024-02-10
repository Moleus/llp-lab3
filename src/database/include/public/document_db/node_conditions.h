#pragma once

#include "node_value_matcher.h"

NodeConditionFunc node_conditions_equals_str(char *expected_str);

NodeConditionFunc node_conditions_parent_equals(node_id_t parent_id);

NodeConditionFunc node_condition_all(void);
