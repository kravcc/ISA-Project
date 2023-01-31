
#define MAX_CERTFILES 10
#define MAX_CERTADDRESSES 10
#include <stdbool.h>


struct par_content {
    bool            is_help;                        // show help
    char*           url;
    char*           feedfile;
    char*           certfile;
    char*           certaddr;
    bool            is_t;                           // -T = include creation or change time
    bool            is_a;                           // -a = include author name or email
    bool            is_u;                           // -u = include URL
    bool            error;
};

struct par_content par_init();
struct par_content parser(int argc, char *argv[]);
void free_par_content(struct par_content *tmp);