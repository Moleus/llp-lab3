#include <assert.h>
#include "private/storage/page.h"
#include "private/storage/page_manager.h"

size_t page_size(Page *self) {
    return self->page_header.page_size;
}

uint32_t page_get_free_space_left(Page *self) {
    return self->page_header.free_space_end_offset - self->page_header.free_space_start_offset;
}

int32_t page_get_payload_available_space(Page *self) {
    return max((int32_t) page_get_free_space_left(self) - (int32_t) sizeof(ItemMetadata), 0);
}

uint32_t page_get_payload_size(uint32_t page_size) {
    return page_size - (uint32_t) HEADER_SIZE;
}

bool page_can_fit_payload(Page *self, uint32_t payload_size) {
    return page_get_payload_available_space(self) >= (int32_t) payload_size;
}

bool page_can_fit_any_payload(Page *self) {
    return page_get_payload_available_space(self) > 0;
}

page_index_t page_get_item_continuation(Page *self, Item *item) {
    ASSERT_ARG_NOT_NULL(self)
    ASSERT_ARG_NOT_NULL(item)

    LOG_DEBUG("Get item continuation. Page id: %d, item id: %d", self->page_header.page_id.id, item->id.item_id);
    ItemMetadata *metadata = get_metadata(self, item->id);
    return metadata->continues_on_page;
}

ItemMetadata *get_metadata(const Page *self, item_index_t item_id) {
    assert(item_id.item_id >= 0);
    ItemMetadata *metadata = (ItemMetadata *) (((uint8_t *)self->page_payload) + ((uint32_t) sizeof(ItemMetadata) * item_id.item_id));
    LOG_DEBUG("Payload address: %p, metadata address: %p, item id: %d, page id: %d", self->page_payload, metadata, item_id.item_id, self->page_header.page_id);
    return metadata;
}

uint8_t *get_item_data_addr(const Page *self, uint32_t data_offset) {
    return ((uint8_t *) self) + data_offset;
}

Item create_item(ItemPayload payload, item_index_t item_id) {
    return (Item) {.is_deleted = false, .id = item_id, .payload = payload};
}

page_index_t page_get_id(const Page *self) {
    return self->page_header.page_id;
}
