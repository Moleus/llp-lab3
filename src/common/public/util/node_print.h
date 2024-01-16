#pragma once

// print information about nodeValue based on type

void print_node_value(NodeValue nodeValue) {
    switch (nodeValue.type) {
        case INT_32:
            printf("int_value: %d\n", nodeValue.int_value);
            break;
        case DOUBLE:
            printf("double_value: %f\n", nodeValue.double_value);
            break;
        case STRING:
            printf("string_value: %s\n", nodeValue.string_value.value);
            break;
        case BOOL:
            printf("bool_value: %d\n", nodeValue.bool_value);
            break;
        case FILE_INFO:
            // name, owner, access_time, mime_type
            printf("file_info_value: %s, %s, %ld, %s\n", nodeValue.file_info_value.name,
                   nodeValue.file_info_value.owner, nodeValue.file_info_value.access_time,
                   nodeValue.file_info_value.mime_type);
            break;
        default:
            printf("unknown type\n");
            break;
    }
}