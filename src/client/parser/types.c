#include "types.h"
#include "assert.h"
#include "../../common/public/util/memory.h"

size_t g_malloc_bytes = 0;

void *my_malloc(size_t size) {
    g_malloc_bytes += size;
    void *ptr = calloc(size, 1);
    return ptr;
}

void print_filter(Filter *pFilter, int level);

Element *create_boolean(bool value) {
    Element *el = my_malloc(sizeof(Element));
    el->type = BOOLEAN_TYPE;
    el->boolean = value;
    return el;
}

Element *create_number(int32_t value) {
    Element *el = my_malloc(sizeof(Element));
    el->type = NUMBER_TYPE;
    el->number = value;
    return el;
}

Element *create_double(double value) {
    Element *el = my_malloc(sizeof(Element));
    el->type = DOUBLE_TYPE;
    el->double_number = value;
    return el;
}

Element *create_string(char *value) {
    Element *el = my_malloc(sizeof(Element));
    el->type = STRING_TYPE;
    el->string = strdup(value);
    return el;
}

Filter *create_filter(char *attribute, int op, Element *value) {
    FilterExpr *filter_expr = my_malloc(sizeof(FilterExpr));
    Filter *filter = my_malloc(sizeof(Filter));
    filter_expr->left.name = strdup(attribute);
    switch (op) {
        case 0:
            filter_expr->operation = EQUALS_OP;
            break;
        case 1:
            filter_expr->operation = NOT_EQUALS_OP;
            break;
        case 2:
            filter_expr->operation = LESS_THAN_OP;
            break;
        case 3:
            filter_expr->operation = GREATER_THAN_OP;
            break;
    }
    filter_expr->type = SELECT_BY_LOGICAL_OP;
    filter_expr->right = value;
    filter->filter = filter_expr;
    return filter;
}

Filter *create_filter_single_value(Element *value) {
    Filter *filter = my_malloc(sizeof(Filter));
    FilterExpr *filter_expr = my_malloc(sizeof(FilterExpr));
    filter_expr->operation = NO_OP;
    filter_expr->type = SELECT_BY_VALUE;
    filter_expr->right = value;
    filter->filter = filter_expr;
    return filter;
}

Filter* create_filter_by_var_name(char *var_name) {
    Filter *filter = my_malloc(sizeof(Filter));
    FilterExpr *filter_expr = my_malloc(sizeof(FilterExpr));
    Element *el = my_malloc(sizeof(Element));
    filter_expr->operation = NO_OP;
    filter_expr->type = SELECT_BY_PROP_NAME;
    filter->filter = filter_expr;
    filter_expr->right = el;
    el->type = STRING_TYPE;
    filter_expr->right->string = strdup(var_name);
    return filter;
}

Filter* create_filter_all() {
    Filter *filter = my_malloc(sizeof(Filter));
    FilterExpr *filter_expr = my_malloc(sizeof(FilterExpr));
    filter_expr->operation = ALL_OP;
    filter_expr->type = SELECT_BY_PROP_NAME;
    filter->filter = filter_expr;
    filter_expr->right = NULL;
    return filter;
}

FunctionType get_function_type(char *func) {
    if (strcmp(func, "update") == 0) {
        return UPDATE_OP;
    } else if (strcmp(func, "create") == 0) {
        return CREATE_OP;
    } else if (strcmp(func, "delete") == 0) {
        return DELETE_OP;
    } else {
        return NOP;
    }
}

void add_node(Query *query, char *node) {
    if (query->nodes != NULL && strcmp(query->nodes->name, "_root") == 0) {
        // filter already applied
        query->nodes->name = strdup(node);
        return;
    }

    ParsedNode *new_node = my_malloc(sizeof(ParsedNode));
    new_node->name = strdup(node);

    ParsedNode *cur_node = query->nodes;
    if (cur_node == NULL) {
        query->nodes = new_node;
        return;
    }
    while (cur_node->next != NULL) {
        cur_node = cur_node->next;
    }
    cur_node->next = new_node;
}

void add_filter(Query *query, Filter *filter) {
    // если фильтр на первой ноде, то ноды еще нет. Нода будет создана
    // TODO: query->nodes is null
    if (query->nodes == NULL) {
        query->nodes = my_alloc(sizeof(ParsedNode));
        query->nodes->name = strdup("_root");
    }
    ParsedNode *cur_node = query->nodes;
    assert(cur_node != NULL);
    while (cur_node->next != NULL) {
        cur_node = cur_node->next;
    }
    Filter *cur_filter = cur_node->filters;
    if (cur_filter == NULL) {
        cur_node->filters = filter;
        return;
    }
    while (cur_filter->next != NULL) {
        cur_filter = cur_filter->next;
    }
    cur_filter->next = filter;

}

void print_tab(int indent_level) {
    for (int i = 0; i < indent_level; i++) {
        printf("\t");
    }
}

void print_element(Element *el) {
    switch (el->type) {
        case BOOLEAN_TYPE:
            printf("%s", el->boolean ? "true" : "false");
            break;
        case NUMBER_TYPE:
            printf("%d", el->number);
            break;
        case DOUBLE_TYPE:
            printf("%f", el->double_number);
            break;
        case STRING_TYPE:
            printf("%s", el->string);
            break;
    }
}

void print_node(ParsedNode *node) {
    int indent_level = 0;
    while (node != NULL) {
        print_tab(indent_level);
        printf("node name: %s\n", node->name);
        if (node->filters != NULL) {
            print_tab(indent_level);
            printf("Filters for node:\n");
            Filter *filter = node->filters;
            while (filter != NULL) {
                print_filter(filter, indent_level);
                if (filter->next != NULL) {
                    printf(", ");
                }
                filter = filter->next;
            }
        }
        if (node->next != NULL) {
            indent_level++;
        }
        node = node->next;
    }
    printf("\n");
}

void print_filter(Filter *pFilter, int level) {
    switch (pFilter->filter->type) {
        case SELECT_BY_PROP_NAME:
            print_tab(level);
            printf("  filter type: select property\n");
            print_tab(level);
            printf("  arg name: %s\n", pFilter->filter->right->string);
            break;
        case SELECT_BY_VALUE:
            print_tab(level);
            printf("  filter type: select element\n");
            print_tab(level);
            printf("  arg value: ");
            print_element(pFilter->filter->right);
            printf("\n");
            break;
        case SELECT_BY_LOGICAL_OP:
            print_tab(level);
            printf("  filter type: select property by condition\n");
            print_tab(level);
            printf("  arg name: %s\n", pFilter->filter->left.name);
            print_tab(level);
            printf("  operation: ");
            switch (pFilter->filter->operation) {
                case EQUALS_OP:
                    printf("==");
                    break;
                case NOT_EQUALS_OP:
                    printf("!=");
                    break;
                case LESS_THAN_OP:
                    printf("<");
                    break;
                case GREATER_THAN_OP:
                    printf(">");
                    break;
                case NO_OP:
                    fprintf(stderr, "Error: NO_OP is not a valid operation\n");
                    exit(1);
                    break;
            }
            printf("\n");
            print_tab(level);
            printf("  arg value: ");
            print_element(pFilter->filter->right);
            printf("\n");
            break;
    }
}


void print_function(FunctionType func) {
    switch (func) {
        case UPDATE_OP:
            printf("update");
            break;
        case CREATE_OP:
            printf("create");
            break;
        case DELETE_OP:
            printf("delete");
            break;
        case NOP:
            break;
    }
}

void print_query(Query query) {
    printf("Parsed Query: \n");
    print_node(query.nodes);
    if (query.func != NOP) {
        printf("Applying function: ");
        print_function(query.func);
        printf("\n");
    }
}
