#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct xml_item {
    char*       name;
    char*       time;
    char*       author;
    char*       url;
};

struct xml_content {
    char*               name;
    int                 num_items;
    struct xml_item*    items;
    bool                error;
};

struct xml_content parse_xml(char *xml);

struct xml_item get_xml_item();

void print_xml_item(struct xml_item *item);

void free_xml_item(struct xml_item *item);

struct xml_content get_xml_content();

void print_xml_content(struct xml_content *content);

void free_xml_content(struct xml_content *content);


