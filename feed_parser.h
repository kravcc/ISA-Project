#define FEEDFILE_BUFFERSIZE 200

struct feed_content {
    int             feed_counter;
    char**          feeds;
    bool            error;
};

void parse(struct feed_content *result, FILE *file);
void print_ff_content(struct feed_content *content);
void free_ff_content(struct feed_content *content);
struct feed_content parse_feedfile(char *file);
struct feed_content feed_init();