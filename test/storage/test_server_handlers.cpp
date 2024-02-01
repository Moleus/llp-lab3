#include "gtest/gtest.h"
#include "common.h"

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

NodesArray* setup_nodes(Document *doc) {
    CreateNodeRequest root_req = (CreateNodeRequest) {
            .parent = NULL_NODE_ID,
            .value = node_value_string_new("/")
    };

    Node root =  {0};
    Result res = document_add_node(doc, &root_req, &root);
    ABORT_IF_FAIL(res, "Failed to add root node");

    char *nodes[] = {
            "ssl root 1705324315 inode/directory",
            "hosts root 1705324315 text/plain",
            "passwd root 1705324315 text/plain"
    };

    int nodes_count = sizeof(nodes) / sizeof(nodes[0]);
    NodesArray *arr = (NodesArray *) my_alloc(sizeof(NodesArray) + sizeof(Node) * nodes_count);
    for (int i = 0; i < nodes_count; i++) {
        CreateNodeRequest req = (CreateNodeRequest) {
                .parent = root.id,
                .value = node_value_string_new(nodes[i])
        };
        res = document_add_node(doc, &req, &(arr->nodes[i]));
        ABORT_IF_FAIL(res, "Failed to add child node");
    }

    return arr;
}

TEST(server_handlers, get_nodes_by_filter_request) {
    remove(FILE_PATH);

    Document* doc = server_init_document(FILE_PATH, PAGE_SIZE);
    NodesArray *arr = setup_nodes(doc);

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
    ASSERT_EQ(response.nodes[0]->id->page_id, arr->nodes[2].id.page_id);
    ASSERT_EQ(response.nodes[0]->id->item_id, arr->nodes[2].id.item_id);
    ASSERT_EQ(response.nodes[0]->value->type, RPC__NODE_VALUE__TYPE__STRING);
    ASSERT_STREQ(response.nodes[0]->value->string_value, "passwd root 1705324315 text/plain");
}

TEST(server, handle_create_node_request) {
    remove(FILE_PATH);


    Document* doc = server_init_document(FILE_PATH, PAGE_SIZE);
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
}
