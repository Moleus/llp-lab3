#include "y.tab.h"

extern size_t g_malloc_bytes;

int main(int argc, char **argv) {
    int status = yyparse();
    print_query(q);
    if (argc > 1) {
        printf("g_malloc_bytes: %zu\n", g_malloc_bytes);
    }
    return status;
}
