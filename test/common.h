#pragma once

extern "C" {
#include "private/storage/page_manager.h"
#include "public/util/common.h"
#include "private/document_db/document.h"
#include "public/util/memory.h"
#include "public/document_db/node_conditions.h"
#include "private/storage/file.h"
}

#define FILE_PATH "/tmp/test.llp"

void remove_file();

void assert_node(Node *node, int page_id, int item_id, int parent_page_id, int parent_item_id, NodeValue value);

NodesArray* setup_nodes(Document *doc);
