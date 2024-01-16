#pragma once

#include "log.h"
#include "stdbool.h"
#include "error.h"

enum ResultStatus {
    RES_OK = 0,
    RES_ERROR = -1
};
// define Result struct with OK and ERROR
typedef struct {
    enum ResultStatus status;
    const char *message;
} Result;


// define OK macro
#define OK (Result) {.status = RES_OK, .message = NULL}

// define ERROR macro
#define ERROR(msg) (Result) {.status = RES_ERROR, .message = (msg)}

static bool result_is_fail__(Result result) {
    return result.status == RES_ERROR;
}

#define RETURN_IF_FAIL(result, return_error_msg) \
    if (result_is_fail__(result)) {      \
        LOG_WARN("%s", (result).message);       \
        return ERROR(return_error_msg);          \
    }

#define ABORT_IF_FAIL(result, return_error_msg)  \
    if (result_is_fail__(result)) {              \
        LOG_ERR("%s", (result).message);         \
        exit(1);                                 \
    }
