#include <stdbool.h>
#include <malloc.h>
#include "public/document_db/node_value_matcher.h"

struct NodeMatcher {
    int conditions_count;
    NodeConditionFunc conditions[];
};

bool node_condition_matches(NodeMatcher *self, Node value) {
    for (int i = 0; i < self->conditions_count; i++) {
        if (!self->conditions[i](value)) {
            return false;
        }
    }
    return true;
}

NodeMatcher *node_condition_new(NodeConditionFunc condition) {
    NodeMatcher *matcher = malloc(sizeof(NodeMatcher));
    matcher->conditions_count = 1;
    matcher->conditions[0] = condition;
    return matcher;
}

NodeMatcher *node_add_condition(NodeMatcher *self, NodeConditionFunc condition) {
    self->conditions_count++;
    self->conditions[self->conditions_count - 1] = condition;
    return self;
}

NodeMatcher *node_remove_last_condition(NodeMatcher *self) {
    self->conditions_count--;
    return self;
}