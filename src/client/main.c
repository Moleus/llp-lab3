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

    char *command = "a/b/d[@x=1][@y=2][@z=hello]\n";
    parser_parse_command(command);
//    return 0;

    ClientService *service = client_service_new(address);

    for (;;) {
//        Query query = parse();

        // hardcoded sample data
        CreateNodeRequest data = {
                .parent = {
                        .page_id = 0,
                        .item_id = 0
                },
                .value = (NodeValue) {
                        .type = FILE_INFO,
                        .file_info_value = {
                                .name = "test.txt",
                                .owner = "test",
                                .access_time = 123456789,
                                .mime_type = "text/plain"
                        }
                }
        };

        run_main_loop(service);

        client_add_node(service, &data);
        sleep(5);
    }
}