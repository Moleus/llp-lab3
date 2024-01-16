#include "y.tab.h"
#include "my_parser.h"

extern size_t g_malloc_bytes;

Query parse() {
    int status = yyparse();
    if (status != 0) {
        printf("failed to parse query\n");
        exit(1);
    }
    print_query(q);
    return q;
}
