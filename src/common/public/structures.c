#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "structures.h"
#include "public/util/log.h"
#include "public/util/memory.h"

NodeValue node_value_string_new(char *string) {
    assert(strlen(string) < 256);
    NodeValue nodeValue = {};
    nodeValue.type = STRING;
    nodeValue.string_value.length = strlen(string);
    strcpy(nodeValue.string_value.value, string);
    return nodeValue;
}

FileInfo parse_file_info(char *file_info) {
    char *name = strtok(file_info, " ");
    char *owner = strtok(NULL, " ");
    char *access_time = strtok(NULL, " ");
    char *mime_type = strtok(NULL, " ");
    return (FileInfo) {
            .name = name,
            .owner = owner,
            .access_time = access_time,
            .mime_type = mime_type
    };
}

bool contains_file_info_attribute(char *attribute) {
    return strcmp(attribute, "name") == 0 || strcmp(attribute, "owner") == 0 || strcmp(attribute, "access_time") == 0 || strcmp(attribute, "mime_type") == 0;
}

void file_info_to_string(FileInfo file_info, char* buffer) {
    sprintf(buffer, "%s %s %s %s", file_info.name, file_info.owner, file_info.access_time, file_info.mime_type);
}

FileInfo file_info_set_attribute(FileInfo file_info, FileInfoAttributes attr, char *value) {
    switch (attr) {
        case FILE_INFO_NAME:
            file_info.name = value;
            break;
        case FILE_INFO_OWNER:
            file_info.owner = value;
            break;
        case FILE_INFO_ACCESS_TIME:
            file_info.access_time = value;
            break;
        case FILE_INFO_MIME_TYPE:
            file_info.mime_type = value;
            break;
        default:
            LOG_ERR("Unknown file info attribute: %d", attr);
            exit(1);
    }
    return file_info;
}

FileInfoAttributes file_info_attribute_from_string(char *attribute) {
    if (strcmp(attribute, "name") == 0) {
        return FILE_INFO_NAME;
    } else if (strcmp(attribute, "owner") == 0) {
        return FILE_INFO_OWNER;
    } else if (strcmp(attribute, "access_time") == 0) {
        return FILE_INFO_ACCESS_TIME;
    } else if (strcmp(attribute, "mime_type") == 0) {
        return FILE_INFO_MIME_TYPE;
    } else {
        LOG_ERR("Unknown file info attribute: %s", attribute);
        exit(1);
    }
}

bool file_info_attribute_matches(FileInfoAttributes attr, char *value, FileInfo file_info) {
    switch (attr) {
        bool result;
        case FILE_INFO_NAME:
            result = (strcmp(file_info.name, value) == 0);
            return result;
        case FILE_INFO_OWNER:
            return strcmp(file_info.owner, value) == 0;
        case FILE_INFO_ACCESS_TIME:
            return strcmp(file_info.access_time, value) == 0;
        case FILE_INFO_MIME_TYPE:
            return strcmp(file_info.mime_type, value) == 0;
        default:
            LOG_ERR("Unknown file info attribute: %d", attr);
            exit(1);
    }
}

NodesArray *nodes_array_new(size_t count) {
    NodesArray *arr = my_alloc(sizeof(NodesArray) + sizeof(Node) * count);
    arr->count = count;
    return arr;
}
