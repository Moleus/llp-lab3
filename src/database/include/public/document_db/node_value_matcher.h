#pragma once

#include <stdbool.h>
#include "public/structures.h"

typedef bool (^NodeConditionFunc) (Node);

typedef struct NodeMatcher NodeMatcher;

typedef struct NodeMatcherArray {
    int matchers_count;
    NodeMatcher *matchers[];
} NodeMatcherArray;

NodeMatcher *node_condition_new(NodeConditionFunc condition);

void node_matcher_destroy(NodeMatcher *self);

NodeMatcher *node_add_condition(NodeMatcher *self, NodeConditionFunc condition);

NodeMatcher *node_remove_last_condition(NodeMatcher *self);

bool node_condition_matches(NodeMatcher *self, Node value);

NodeMatcherArray *node_matcher_array_new(NodeConditionFunc conditions[], int count);

void node_matcher_array_destroy(NodeMatcherArray *self);