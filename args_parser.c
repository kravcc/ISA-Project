#include "args_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct par_content par_init()
{
    struct par_content tmp;
    tmp.feedfile = NULL;
    tmp.is_a = false;
    tmp.is_help = false;
    tmp.is_t = false;
    tmp.is_u = false;
    tmp.url = NULL;
    tmp.certaddr = NULL;
    tmp.certfile = NULL;
    tmp.error = false;
    return tmp;
}

void free_par_content(struct par_content *tmp) {
    if (tmp->url != NULL) {
        free(tmp->url);
        tmp->url = NULL;
    }
    if (tmp->feedfile != NULL) {
        free(tmp->feedfile);
        tmp->feedfile = NULL;
    }
    if (tmp->certaddr != NULL) {
        free(tmp->certaddr);
        tmp->certaddr = NULL;
    }
    if (tmp->certfile != NULL) {
        free(tmp->certfile);
        tmp->certfile = NULL;
    }
}


struct par_content parser(int argc, char *argv[])
{
    struct par_content tmp = par_init();
    int i = 1;
    while(i < argc){
        if (argv == NULL || argv[i][0] == '\0' || (argv[i][0] == '-' && argv[i][1] == '\0')) {
            tmp.error = true;
            printf("Invalid argument");
            return tmp;
        } else if (argv[i][0] != '-') {
            if (tmp.url == NULL && tmp.feedfile == NULL){
                tmp.url = (char*) malloc((strlen(argv[i])+1) * sizeof(char));
                strcpy(tmp.url, argv[i]);
            } else {
                tmp.error = true;
                printf("Zadano vice zdroju feedu!\n");
                return tmp;
            }
        } else if (argv[i][2] == '\0') {
            switch (argv[i][1])
            {
                // feedfile case
                case 'f':
                    if (tmp.url == NULL && tmp.feedfile == NULL)
                    {
                        if (i + 1 == argc){
                            tmp.error = true;
                            printf("Chybi feedfile\n");
                            return tmp;
                        }
                        i++;
                        tmp.feedfile = (char*) malloc((strlen(argv[i])+1) * sizeof(char));
                        strcpy(tmp.feedfile, argv[i]);
                    } else {
                        tmp.error = true;
                        printf("Zadano vice zdroju feedu!\n");
                        return tmp;
                    }
                    break;
                // certifile case
                case 'c':
                    if (tmp.certfile == NULL)
                    {
                        if (i + 1 == argc){
                            tmp.error = true;
                            printf("Chybi certfile\n");
                            return tmp;
                        }
                        i++;
                        tmp.certfile = (char*) malloc((strlen(argv[i])+1) * sizeof(char));
                        strcpy(tmp.certfile, argv[i]);
                    }
                    break;
                // certaddr case
                case 'C':
                    if (tmp.certaddr == NULL)
                    {
                        if (i + 1 == argc){
                            tmp.error = true;
                            printf("Chybi certadr\n");
                            return tmp;
                        }
                        i++;
                        tmp.certaddr = (char*) malloc((strlen(argv[i])+1) * sizeof(char));
                        strcpy(tmp.certaddr, argv[i]);
                    }
                    break;
                // time case
                case 'T':
                    tmp.is_t = true;
                break;
                // help case
                case 'h':
                    tmp.is_help = true;
                break;
                // author case
                case 'a':
                    tmp.is_a = true;
                break;
                // url case
                case 'u':
                    tmp.is_u = true;
                break;
                // unknown argument - error
                default:
                    tmp.error = true;
                    printf("Invalid argument\n");
                    return tmp;
                break;
            }
        } else {
            tmp.error = true;
            printf("Invalid argument\n");
            return tmp;
        }
        i++;
    }
    return tmp;
};
