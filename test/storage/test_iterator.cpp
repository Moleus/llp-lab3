#include "gtest/gtest.h"

extern "C" {
#include "private/storage/page_manager.h"
#include "public/util/common.h"
#include "private/document_db/document.h"
#include "public/util/memory.h"
#include "public/document_db/node_conditions.h"
}

#define PAGE_SIZE 4096
#define SIGNATURE 0x12345678

#define FILE_PATH "/tmp/test.llp"

/*
init PageManager. put several items. Then init item iterator and check that it iterates over all of them
 */
TEST(test_iterator, basic_iteration) {
    char *tmpfilename = tmpnam(NULL);

    PageManager *pm = page_manager_new();
    page_manager_init(pm, tmpfilename, PAGE_SIZE, SIGNATURE);
    Page *page = page_manager_get_current_free_page(pm);
    char *d1 = "test 1_";
    char *d2 = "test 2__";
    char *d3 = "test 3___";
    ItemPayload p1 = {
            .size = static_cast<uint32_t>(strlen(d1)),
            .data = d1
    };
    ItemPayload p2 = {
            .size = static_cast<uint32_t>(strlen(d2)),
            .data = d2
    };
    ItemPayload p3 = {
            .size = static_cast<uint32_t>(strlen(d3)),
            .data = d3
    };
    ItemAddResult result;
    page_manager_put_item(pm, page, p1, &result);
    page_manager_put_item(pm, page, p2, &result);
    page_manager_put_item(pm, page, p3, &result);
    Item item;
    ItemIterator *it = page_manager_get_items(pm, &item);
    ASSERT_TRUE(item_iterator_has_next(it));

    item_iterator_next(it, &item);
    ASSERT_EQ(item.payload.size, p1.size);
    ASSERT_EQ(memcmp(item.payload.data, p1.data, p1.size), 0);
    ASSERT_TRUE(item_iterator_has_next(it));

    item_iterator_next(it, &item);
    ASSERT_EQ(item.payload.size, p2.size);
    ASSERT_EQ(memcmp(item.payload.data, p2.data, p2.size), 0);
    ASSERT_TRUE(item_iterator_has_next(it));

    item_iterator_next(it, &item);
    ASSERT_EQ(item.payload.size, p3.size);
    ASSERT_EQ(memcmp(item.payload.data, p3.data, p3.size), 0);

    ASSERT_FALSE(item_iterator_has_next(it));


    item_iterator_destroy(it);
    page_manager_destroy(pm);
    remove(tmpfilename);
}