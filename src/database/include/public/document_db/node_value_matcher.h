#pragma once

#include <stdbool.h>
#include "public/structures.h"

typedef bool (^NodeConditionFunc) (Node);

typedef struct NodeMatcher NodeMatcher;

NodeMatcher *node_condition_new(NodeConditionFunc condition);

NodeMatcher *node_add_condition(NodeMatcher *self, NodeConditionFunc condition);

NodeMatcher *node_remove_last_condition(NodeMatcher *self);

bool node_condition_matches(NodeMatcher *self, Node value);
