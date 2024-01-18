#include <string.h>
#include <assert.h>
#include "structures.h"

NodeValue node_value_string_new(char *string) {
    assert(strlen(string) < 256);
    NodeValue nodeValue = {};
    nodeValue.type = STRING;
    nodeValue.string_value.length = strlen(string);
    strcpy(nodeValue.string_value.value, string);
    return nodeValue;
}