struct url_content {
    bool            is_https;
    bool            is_port;
    char*           host;
    char*           port;
    char*           path;
    bool            error;
};

struct url_content parse_url(char *url);
void free_url_content(struct url_content *content) ;
struct url_content url_init();
void print_url_content(struct url_content *content);