#include <stdbool.h>
#include <Block.h>
#include "public/document_db/node_value_matcher.h"
#include "public/util/memory.h"
#include "public/util/helpers.h"

struct NodeMatcher {
    int conditions_count;
    NodeConditionFunc conditions[10];
};

bool node_condition_matches(NodeMatcher *self, Node value) {
    for (int i = 0; i < self->conditions_count; i++) {
        if (!self->conditions[i](value)) {
            return false;
        }
    }
    return true;
}

NodeMatcher *node_matcher_new(NodeConditionFunc condition) {
    NodeMatcher *matcher = my_alloc(sizeof(NodeMatcher) * 1);
    matcher->conditions_count = 1;
    matcher->conditions[0] = condition;
    return matcher;
}

NodeMatcher *node_add_condition(NodeMatcher *self, NodeConditionFunc condition) {
    if (self->conditions_count > 8) {
        ABORT_EXIT(INTERNAL_LIB_ERROR, "NodeMatcher conditions exceeded maximum value of 10");
    }
    self->conditions_count++;
    self->conditions[self->conditions_count - 1] = condition;
    return self;
}

NodeMatcher *node_remove_last_condition(NodeMatcher *self) {
    Block_release(self->conditions[self->conditions_count - 1]);
    self->conditions_count--;
    return self;
}

NodeMatcherArray *node_matcher_array_new(NodeConditionFunc conditions[], int count) {
    NodeMatcherArray *array = my_alloc(sizeof(NodeMatcher*) * count + sizeof(NodeMatcherArray));
    array->matchers_count = count;
    for (int i = 0; i < count; i++) {
        array->matchers[i] = node_matcher_new(conditions[i]);
    }
    return array;
}

void node_matcher_destroy(NodeMatcher *self) {
    for (int i = 0; i < self->conditions_count; i++) {
        Block_release(self->conditions[i]);
    }
    free(self);
}

void node_matcher_array_destroy(NodeMatcherArray *self) {
    for (int i = 0; i < self->matchers_count; i++) {
        node_matcher_destroy(self->matchers[i]);
    }
    free(self);
}