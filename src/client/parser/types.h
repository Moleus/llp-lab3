#ifndef PARSER_TYPES_H
#define PARSER_TYPES_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "string.h"

#define MAX_STRING_SIZE 50
#define MAX_PATH_DEPTH 10

typedef enum {
    NOP = 0,
    UPDATE_OP = 1,
    CREATE_OP,
    DELETE_OP,
} FunctionType;

typedef enum {
    NO_OP = 0,
    EQUALS_OP = 1,
    NOT_EQUALS_OP,
    LESS_THAN_OP,
    GREATER_THAN_OP,
    ALL_OP
} LogicalOperation;

typedef enum {
    BOOLEAN_TYPE = 1,
    NUMBER_TYPE,
    DOUBLE_TYPE,
    STRING_TYPE
} ParsedValueType;

typedef struct {
    ParsedValueType type;
    union {
        bool boolean;
        int32_t number;
        double double_number;
        char *string;
    };
} Element;

typedef struct {
    char *name;
} FilterTarget;

typedef enum {
    SELECT_BY_PROP_NAME,
    SELECT_BY_VALUE,
    SELECT_BY_LOGICAL_OP,
} FilterExprType;

typedef struct {
    LogicalOperation operation;
    FilterTarget left;
    Element *right;
    FilterExprType type;
    bool is_single_value;
} FilterExpr;

typedef struct {
    ParsedValueType type;
    char *name;
} Property;

typedef struct Filter {
    union {
        Property* property; // when adding new property
        FilterExpr* filter; // when querying
    };
    struct Filter *next;
} Filter;

typedef enum {
    SINGLE_ATTRIBUTE = 1,
    ASTERISK_PATH,
} PathType;

typedef struct ParsedNode {
    char *name;
    struct ParsedNode *next;
    Filter* filters;
} ParsedNode;

typedef struct {
    FunctionType func;
    ParsedNode *nodes;
} Query;

Element *create_boolean(bool value);

Element *create_number(int32_t value);

Element *create_double(double value);

Element *create_string(char *value);

Filter *create_filter(char *attribute, int op, Element *value);

Filter *create_filter_single_value(Element *value);

Filter* create_filter_by_var_name(char *var_name);

Filter* create_filter_all();

FunctionType get_function_type(char *func);

void print_element(Element *el);

void print_query(Query query);

void add_node(Query *query, char *node);

void add_filter(Query *query, Filter *filter);
#endif //PARSER_TYPES_H
