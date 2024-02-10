#include "gtest/gtest.h"
#include "common.h"

extern "C" {
#include "helpers.h"
#include "parser/types.h"
#include "public/util/memory.h"
#include "converters.h"
#include "parser/my_parser.h"
}

#define PAGE_SIZE 4096
#define SIGNATURE 0x12345678

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

TEST(client, simple_command) {
    char *command = "ssl[*]\n";
    Query *query = parser_parse_command(command);

    Rpc__FilterChain *chain = convertNodesToFilterChain(query->nodes);

    ASSERT_EQ(query->func, NOP);
    ASSERT_EQ(chain->n_filters, 3);
    ASSERT_STREQ(chain->filters[0]->string_argument, "/");
    ASSERT_STREQ(chain->filters[1]->string_argument, "ssl");
    ASSERT_EQ(chain->filters[2]->type, RPC__FILTER__TYPE__ALL);
}

TEST(client, parse_multiple_filters) {
    char *command = "create(ssl[@name=a1][@owner=root][@access_time=1705324315][@mime_type=text.plain])\n";

    Query *query = parser_parse_command(command);
    Filter *f = query->nodes->filters;

    ASSERT_EQ(query->func, CREATE_OP);
    ASSERT_STREQ(query->nodes->name, "ssl");

    ASSERT_STREQ(f->filter->left.name, "name");
    ASSERT_STREQ(f->filter->right->string, "a1");
    f = f->next;
    ASSERT_STREQ(f->filter->left.name, "owner");
    ASSERT_STREQ(f->filter->right->string, "root");
    f = f->next;
    ASSERT_STREQ(f->filter->left.name, "access_time");
    ASSERT_STREQ(f->filter->right->string, "1705324315");
    f = f->next;
    ASSERT_STREQ(f->filter->left.name, "mime_type");
    ASSERT_STREQ(f->filter->right->string, "text.plain");

    ASSERT_EQ(f->next, nullptr);
}