#include "gtest/gtest.h"

extern "C" {
#include "helpers.h"
#include "parser/types.h"
#include "public/util/memory.h"
#include "converters.h"
}

#define PAGE_SIZE 4096
#define SIGNATURE 0x12345678

//typedef struct ParsedNode {
//    char *name;
//    struct ParsedNode *next;
//    Filter* filters;
//} ParsedNode;

TEST(client, convertNodesToFilterChain) {
    // /etc/local/hosts
    ParsedNode n1 = {.name = "etc", .next = NULL};
    ParsedNode n2 = {.name = "local", .next = NULL};
    ParsedNode n3 = {.name = "hosts", .next = NULL};

    n1.next = &n2;
    n2.next = &n3;

    char *names[] = {"/", "etc", "local", "hosts"};

    Rpc__FilterChain *chain = convertNodesToFilterChain(&n1);
    ASSERT_EQ(chain->n_filters, 4);
    for (int i = 0; i < chain->n_filters; i++) {
        ASSERT_EQ(chain->filters[i]->type, RPC__FILTER__TYPE__EQUAL);
        ASSERT_EQ(chain->filters[i]->field_name_case, RPC__FILTER__FIELD_NAME_NAME);
        ASSERT_EQ(chain->filters[i]->argument_case, RPC__FILTER__ARGUMENT_STRING_ARGUMENT);
        ASSERT_EQ(chain->filters[i]->string_argument, names[i]);
        ASSERT_EQ(strcmp(chain->filters[i]->name, "name"), 0);
    }
}