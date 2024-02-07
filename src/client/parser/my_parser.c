#include "gen/parser.h"
#include "gen/lexer.h"
#include "../../common/public/util/memory.h"

extern size_t g_malloc_bytes;

Query *parser_parse_command(char *command) {
    yy_scan_string(command);

    // copy query
    Query *l_q = my_alloc(sizeof(Query));

    int status = yyparse(l_q);
    if (status != 0) {
        printf("failed to parse query\n");
        exit(1);
    }
//    print_query(*l_q);
    return l_q;
}
