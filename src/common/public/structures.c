#include <string.h>
#include "structures.h"
#include "util/memory.h"

NodeValue *node_value_string_new(char *string) {
    NodeValue *nodeValue = my_alloc(sizeof(NodeValue) + strlen(string) + 1);
    nodeValue->type = STRING;
    nodeValue->string_value.length = strlen(string);
    strcpy(nodeValue->string_value.value, string);
    return nodeValue;
}

Node *node_string_new(char *string) {
    Node *node = my_alloc(sizeof(Node) + strlen(string) + 1);
    node->value.type = STRING;
    node->value.string_value.length = strlen(string);
    strcpy(node->value.string_value.value, string);
    return node;
}