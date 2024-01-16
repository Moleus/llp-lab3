#include "gen/parser.h"

extern size_t g_malloc_bytes;

int main(int argc, char **argv) {
    Query q;
    int status = yyparse(&q);
    print_query(q);
    if (argc > 1) {
        printf("g_malloc_bytes: %zu\n", g_malloc_bytes);
    }
    return status;
}
