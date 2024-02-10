#include "gtest/gtest.h"
#include "common.h"

extern "C" {
#include "private/storage/page_manager.h"
#include "private/document_db/document.h"
#include "public/util/memory.h"
#include "public/document_db/node_conditions.h"
#include "server.h"
}

#define PAGE_SIZE 4096
#define SIGNATURE 0x12345678

TEST(server_handlers, get_nodes_by_filter_request) {
    char *tmpfilename = tmpnam(NULL);

    Document* doc = server_init_document(tmpfilename, PAGE_SIZE);
    NodesArray *arr = setup_nodes(doc);

    Rpc__Filter root_filter = RPC__FILTER__INIT;
    root_filter.type = RPC__FILTER__TYPE__EQUAL;
    root_filter.argument_case = RPC__FILTER__ARGUMENT_STRING_ARGUMENT;
    root_filter.string_argument = "base";
    root_filter.name = "name";
    root_filter.field_name_case = RPC__FILTER__FIELD_NAME_NAME;

    Rpc__Filter filter = RPC__FILTER__INIT;
    filter.type = RPC__FILTER__TYPE__EQUAL;
    filter.argument_case = RPC__FILTER__ARGUMENT_STRING_ARGUMENT;
    filter.string_argument = "passwd";
    filter.name = "name";
    filter.field_name_case = RPC__FILTER__FIELD_NAME_NAME;

    Rpc__FilterChain chain = RPC__FILTER_CHAIN__INIT;
    chain.n_filters = 2;
    chain.filters = static_cast<Rpc__Filter **>(my_alloc(sizeof(Rpc__Filter *) * 2));
    chain.filters[0] = &root_filter;
    chain.filters[1] = &filter;

    Rpc__Nodes response;

    handle_get_nodes_by_filter_request(&chain, &response);

    ASSERT_EQ(response.n_nodes, 1);
    ASSERT_EQ(response.nodes[0]->id->page_id, arr->nodes[2].id.page_id);
    ASSERT_EQ(response.nodes[0]->id->item_id, arr->nodes[2].id.item_id);
    ASSERT_EQ(response.nodes[0]->value->type, RPC__NODE_VALUE__TYPE__STRING);
    ASSERT_STREQ(response.nodes[0]->value->string_value, "passwd root 1705324315 text/plain");

    document_destroy(doc);
    remove(tmpfilename);
}

TEST(server, get_all_children_of_node) {
    char *tmpfilename = tmpnam(NULL);

    Document* doc = server_init_document(tmpfilename, PAGE_SIZE);
    NodesArray *arr = setup_nodes(doc);

    Rpc__Filter root_filter = RPC__FILTER__INIT;
    root_filter.type = RPC__FILTER__TYPE__EQUAL;
    root_filter.argument_case = RPC__FILTER__ARGUMENT_STRING_ARGUMENT;
    root_filter.string_argument = "base";
    root_filter.name = "name";
    root_filter.field_name_case = RPC__FILTER__FIELD_NAME_NAME;

    Rpc__Filter filter = RPC__FILTER__INIT;
    filter.type = RPC__FILTER__TYPE__EQUAL;
    filter.argument_case = RPC__FILTER__ARGUMENT_STRING_ARGUMENT;
    filter.string_argument = "ssl";
    filter.name = "name";
    filter.field_name_case = RPC__FILTER__FIELD_NAME_NAME;

    Rpc__Filter all_filter = RPC__FILTER__INIT;
    all_filter.type = RPC__FILTER__TYPE__ALL;

    Rpc__FilterChain chain = RPC__FILTER_CHAIN__INIT;
    chain.n_filters = 3;
    chain.filters = static_cast<Rpc__Filter **>(my_alloc(sizeof(Rpc__Filter *) * chain.n_filters));
    chain.filters[0] = &root_filter;
    chain.filters[1] = &filter;
    chain.filters[2] = &all_filter;

    Rpc__Nodes response;

    handle_get_nodes_by_filter_request(&chain, &response);

    ASSERT_EQ(response.n_nodes, 2);
    ASSERT_EQ(response.nodes[0]->id->page_id, arr->nodes[3].id.page_id);
    ASSERT_EQ(response.nodes[0]->id->item_id, arr->nodes[3].id.item_id);
    ASSERT_EQ(response.nodes[0]->value->type, RPC__NODE_VALUE__TYPE__STRING);
    ASSERT_STREQ(response.nodes[0]->value->string_value, "cert.pem root 1705324315 text/plain");

    ASSERT_EQ(response.nodes[1]->id->page_id, arr->nodes[4].id.page_id);
    ASSERT_EQ(response.nodes[1]->id->item_id, arr->nodes[4].id.item_id);
    ASSERT_EQ(response.nodes[1]->value->type, RPC__NODE_VALUE__TYPE__STRING);
    ASSERT_STREQ(response.nodes[1]->value->string_value, "private root 1705324315 inode/directory");

    document_destroy(doc);
    remove(tmpfilename);
}

TEST(server, filter_by_field) {
    char *tmpfilename = tmpnam(NULL);

    Document* doc = server_init_document(tmpfilename, PAGE_SIZE);
    NodesArray *arr = setup_nodes(doc);

    Rpc__Filter root_filter = RPC__FILTER__INIT;
    root_filter.type = RPC__FILTER__TYPE__EQUAL;
    root_filter.argument_case = RPC__FILTER__ARGUMENT_STRING_ARGUMENT;
    root_filter.string_argument = "base";
    root_filter.name = "name";
    root_filter.field_name_case = RPC__FILTER__FIELD_NAME_NAME;

    Rpc__Filter filter = RPC__FILTER__INIT;
    filter.type = RPC__FILTER__TYPE__EQUAL;
    filter.argument_case = RPC__FILTER__ARGUMENT_STRING_ARGUMENT;
    filter.string_argument = "ssl";
    filter.name = "name";
    filter.field_name_case = RPC__FILTER__FIELD_NAME_NAME;

    Rpc__Filter mime_filter = RPC__FILTER__INIT;
    mime_filter.type = RPC__FILTER__TYPE__EQUAL;
    mime_filter.argument_case = RPC__FILTER__ARGUMENT_STRING_ARGUMENT;
    mime_filter.string_argument = "inode/directory";
    mime_filter.name = "mime_type";
    mime_filter.field_name_case = RPC__FILTER__FIELD_NAME_NAME;

    Rpc__FilterChain chain = RPC__FILTER_CHAIN__INIT;
    chain.n_filters = 3;
    chain.filters = static_cast<Rpc__Filter **>(my_alloc(sizeof(Rpc__Filter *) * chain.n_filters));
    chain.filters[0] = &root_filter;
    chain.filters[1] = &filter;
    chain.filters[2] = &mime_filter;

    Rpc__Nodes response;

    handle_get_nodes_by_filter_request(&chain, &response);

    ASSERT_EQ(response.n_nodes, 1);
    ASSERT_EQ(response.nodes[0]->id->page_id, arr->nodes[4].id.page_id);
    ASSERT_EQ(response.nodes[0]->id->item_id, arr->nodes[4].id.item_id);
    ASSERT_EQ(response.nodes[0]->value->type, RPC__NODE_VALUE__TYPE__STRING);
    ASSERT_STREQ(response.nodes[0]->value->string_value, "private root 1705324315 inode/directory");

    document_destroy(doc);
    remove(tmpfilename);
}

TEST(server, handle_create_node_request) {
    char *tmpfilename = tmpnam(NULL);

    Document* doc = server_init_document(tmpfilename, PAGE_SIZE);
    NodesArray *arr = setup_nodes(doc);

    CreateNodeRequest req = (CreateNodeRequest) {
        .parent = (node_id_t ) {.page_id=0, .item_id=0},
        .value = (NodeValue) {
            .type = STRING,
            .string_value = (String) {
                .length = 1,
                .value = "a"
            }
        }
    };
    Rpc__CreateNodeRequest *pRequest = convert_to_rpc_CreateNodeRequest(req);

    Rpc__Node response = {0};

    handle_create_node_request(pRequest, &response);

    ASSERT_EQ(response.parent_id->page_id, req.parent.page_id);
    ASSERT_EQ(response.parent_id->item_id, req.parent.item_id);
    ASSERT_EQ(response.value->type, RPC__NODE_VALUE__TYPE__STRING);
    for (int i = 0; i < req.value.string_value.length; i++) {
        ASSERT_EQ(response.value->string_value[i], req.value.string_value.value[i]);
    }

    document_destroy(doc);
    remove(tmpfilename);
}
