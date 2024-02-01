#include "gtest/gtest.h"

extern "C" {
#include "private/storage/page_manager.h"
#include "public/util/common.h"
#include "private/document_db/document.h"
#include "public/util/memory.h"
#include "public/document_db/node_conditions.h"
#include "server.h"
}

#define PAGE_SIZE 4096
#define SIGNATURE 0x12345678

#define FILE_PATH "/tmp/test.llp"

// creates document
// adds 3 nodes to it: a, b, c
// calls handler with filterChain
TEST(server_handlers, get_nodes_by_filter_request) {
    remove(FILE_PATH);

    Document* doc = server_init_document(FILE_PATH, PAGE_SIZE);

    CreateNodeRequest root_req = (CreateNodeRequest) {
            .parent = NULL_NODE_ID,
            .value = node_value_string_new("/")
    };

    Node root =  {0};
    Result res = document_add_node(doc, &root_req, &root);
    ASSERT_EQ(res.status, RES_OK);

    char *nodes[] = {
            "ssl root 1705324315 inode/directory",
            "hosts root 1705324315 text/plain",
            "passwd root 1705324315 text/plain"
    };

    int nodes_count = sizeof(nodes) / sizeof(nodes[0]);
    Node nodes_created[nodes_count];
    for (int i = 0; i < nodes_count; i++) {
        CreateNodeRequest req = (CreateNodeRequest) {
                .parent = root.id,
                .value = node_value_string_new(nodes[i])
        };
        res = document_add_node(doc, &req, &nodes_created[i]);
        ASSERT_EQ(res.status, RES_OK);
    }

    Rpc__Filter root_filter = RPC__FILTER__INIT;
    root_filter.type = RPC__FILTER__TYPE__EQUAL;
    root_filter.argument_case = RPC__FILTER__ARGUMENT_STRING_ARGUMENT;
    root_filter.string_argument = "/";
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
    ASSERT_EQ(response.nodes[0]->id->page_id, nodes_created[2].id.page_id);
    ASSERT_EQ(response.nodes[0]->id->item_id, nodes_created[2].id.item_id);
    ASSERT_EQ(response.nodes[0]->value->type, RPC__NODE_VALUE__TYPE__STRING);
    ASSERT_STREQ(response.nodes[0]->value->string_value, "passwd root 1705324315 text/plain");
}