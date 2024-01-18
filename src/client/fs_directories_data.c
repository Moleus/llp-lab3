
#include "converters.h"
#include "public/util/memory.h"

typedef struct {
    char* name;
    char* owner;
    char* access_time;
    char* mime_type;
} FileInfo;

char *files[] = {
        "ssl root 1705324315 inode/directory",
        "timezone root 1705324315 text/plain",
        "hosts root 1705324315 text/plain",
        "passwd root 1705324315 text/plain",
        "group root 1705324315 text/plain",
        "fstab root 1705324315 text/plain",
        "shadow root 1705324315 text/plain",
        "gshadow root 1705324315 text/plain",
};

// each element has information about parent and file system information
CreateNodeRequest* generate_requests() {
    // hardcoded sample data
    // root
    CreateNodeRequest root = {.parent = {.page_id = -1, .item_id = -1}, .value = node_value_string_new("/ root 1705324315 inode/directory")};

    CreateNodeRequest *requests = my_alloc(sizeof(CreateNodeRequest) * 9);
    requests[0] = root;

    for (int i = 0; i < 8; i++) {
        CreateNodeRequest request = {.parent = {.page_id = 0, .item_id = 0}, .value = node_value_string_new(files[i])};
        requests[i+1] = request;
    }
    return requests;
}
