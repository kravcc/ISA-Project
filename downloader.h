struct down_content {
    char*           content;
    bool            error;
};

struct down_content secure_connect(char* url);
void free_dw_content(struct down_content *content);
void set_certificates(char *certfile_path, char* certadress_path);