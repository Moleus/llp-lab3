#include <unistd.h>
#include "client.h"
#include "public/util/log.h"
#include "parser/types.h"
#include "parser/my_parser.h"


unsigned char log_level = DEBUG;

// read from input using parser
// then call net_client functions based on the query
int main(int argc, char **argv) {
    char *address = "127.0.0.1:9090";

    char *command = "a/b/d\n";
    parser_parse_command(command);
    return 0;

    ClientService *service = client_service_new(address);

    for (;;) {
//        Query query = parse();

        // hardcoded sample data
        CreateFileNodeRequest data = {
                .parent_id = {
                        .page_id = 0,
                        .item_id = 0
                },
                .file_info = {
                        .name = "test.txt",
                        .owner = "test",
                        .access_time = 123456789,
                        .mime_type = "text/plain"
                }
        };

        run_main_loop(service);

        client_add_fileNode(service, &data);
        sleep(5);
    }
}