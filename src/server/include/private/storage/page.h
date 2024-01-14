#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "public/storage/page.h"

#define HEADER_SIZE sizeof(PageHeader)

typedef struct __attribute__((packed)) page_index_t {
    int32_t id;
} page_index_t;
#define NULL_PAGE_INDEX (page_index_t) {.id = -1}

static inline page_index_t page_id(int32_t value) {
    return (page_index_t) {.id = value};
}

static inline page_index_t next_page(page_index_t self) {
    return (page_index_t) {.id = self.id + 1};
}

typedef struct __attribute__((packed)) item_index_t {
    page_index_t page_id;
    int32_t item_id;
} item_index_t;

static inline item_index_t item_id(page_index_t page_id, int32_t value) {
    return (item_index_t) {.page_id = page_id, .item_id = value};
}

static inline item_index_t next_item(item_index_t self) {
    return (item_index_t) {.page_id = self.page_id, .item_id = self.item_id + 1};
}

//#define NULL_ITEM (Item){.size=0, .data = NULL, .index_in_page = 0, .is_deleted = true}

struct __attribute__((packed)) PageHeader {
    page_index_t page_id;
    uint32_t page_size;
    // offset of this page in file
    item_index_t next_item_id;
    uint32_t items_count;
    // these two fields describe a range of free space [start; end]
    uint32_t free_space_start_offset;
    uint32_t free_space_end_offset;
};

typedef struct AllocatedPage AllocatedPage;

struct AllocatedPage {
    Page *rawPage;
    AllocatedPage *next;
};

struct __attribute__((packed)) Page {
    PageHeader page_header;
    uint8_t page_payload[];
};

typedef struct {
    bool complete;
    uint32_t bytes_written;
} ItemWriteStatus;

// we store it on disk
typedef struct __attribute__((packed)) ItemMetadata {
    item_index_t item_id;
    uint32_t data_offset;
    uint32_t size;
    bool is_deleted;
    page_index_t continues_on_page;
} ItemMetadata;

// don't store on disk
struct Item {
    item_index_t id;
    bool is_deleted;
    ItemPayload payload;
};

typedef struct {
    uint32_t metadata_offset_in_page;
    ItemMetadata metadata;
    ItemWriteStatus write_status;
} ItemAddResult;

Page *page_new(page_index_t page_id, uint32_t page_size);

void page_destroy(Page *self);

// payload size
uint32_t page_get_payload_size(uint32_t page_size);

Result page_add_item(Page *self, ItemPayload payload, page_index_t continues_on_page, ItemAddResult *item_add_result);

Result page_delete_item(Page *self, Item *item);

Result page_get_item(Page *self, item_index_t item_id, Item *item);

uint32_t page_get_free_space_left(Page *self);

int32_t page_get_payload_available_space(Page *self);

page_index_t page_get_item_continuation(Page *self, Item *item);

ItemMetadata *get_metadata(const Page *self, item_index_t item_id);

uint8_t *get_item_data_addr(const Page *self, uint32_t data_offset);

Item create_item(ItemPayload payload, item_index_t item_id);

page_index_t page_get_id(const Page *self);
