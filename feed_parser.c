#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "feed_parser.h"


void parse(struct feed_content *result, FILE *file)
{
    bool comment = false;
    bool feed_line = false; 
    char c;
    int j = 0;
    char buffer[FEEDFILE_BUFFERSIZE];

    while(1){

        if (file != NULL) {
            c = getc(file);
        } else {
            return;
        }

        if ((c == EOF || c == '\n' || isspace(c)) && feed_line) {
            buffer[j] = '\0';
            strcpy(result->feeds[result->feed_counter-1], buffer);
            buffer[0] = '\0';
            j = 0;
            feed_line = false;
        }

        if (c == EOF) {
            break;
        }
        else if (c == '\n') {
            comment = false;
        }
        else if (isspace(c)) {
            continue;
        }
        else if (c == '#' && !feed_line && !comment) {
            comment = true;
        }
        else {
            if (comment) {
                continue;
            }
            else if (feed_line) {
                buffer[j] = c;
                j++;
            }
            else {
                feed_line = true;
                buffer[j] = c;
                j++;

                result->feeds = (char**) realloc(result->feeds, (result->feed_counter+1) * sizeof(char *));
                result->feeds[result->feed_counter] = (char *) malloc((FEEDFILE_BUFFERSIZE+1) * sizeof(char));
                result->feed_counter++;
            }
        }
    }
}


void free_ff_content(struct feed_content *content) {
    if (content->feed_counter > 0) {
        for (int i = 0; i < content->feed_counter; i++) {
            free(content->feeds[i]);
            content->feeds[i] = NULL;
        }
        free(content->feeds);
        content->feeds = NULL;
    }
}

struct feed_content parse_feedfile(char *file) {

    struct feed_content tmp = feed_init();

    FILE *f = fopen(file, "r");
    if (f == NULL) {
        tmp.error = true;
        return tmp;
    }
    
    parse(&tmp, f);

    fclose(f);

    return tmp;
}

struct feed_content feed_init() {
    struct feed_content result;
    result.feed_counter = 0;
    result.feeds = NULL;
    result.error = false;
    return result;
}