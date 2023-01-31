#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "url_parser.h"

struct url_content url_init() 
{
    struct url_content tmp;
    tmp.is_https = false;
    tmp.host = NULL;
    tmp.port = NULL;
    tmp.path = NULL;
    tmp.error = false;
    tmp.is_port = false;
    return tmp;
}

void free_url_content(struct url_content *content) 
{
    if (content->host != NULL) {
        free(content->host);
        content->host = NULL;
    }
    if (content->port != NULL) {
        free(content->port);
        content->port = NULL;
    }
    if (content->path != NULL) {
        free(content->path);
        content->path = NULL;
    }
}

struct url_content parse_url(char *url)
{
    struct url_content result = url_init();

    if (strlen(url) < 10)
    {
        result.error = true;
        printf("Invalid URL\n");
        return result;
    }
    // http or https
    if (!(strncmp(url, "https://",8)) || !(strncmp(url, "HTTPS://",8)))
    {
        result.is_https = true;
    } else if (strncmp(url, "http://", 7) && strncmp(url, "HTTP://", 7)) {
        result.error = true;
        printf("Url neni https ani http\n");
        return result;
    }
    
    char c;
    int hostname_len = 20;
    int i = 0;
    int j;
    if (result.is_https == true)
    {
        j = 8;
    } else {
        j = 7;
    }
    
    char *hostname = (char *) malloc(hostname_len * sizeof(char));
    
    while (1) // get hostname
    {
        c = url[j];
        
        if ( c == '/' || c == '\0' || c == ':')
        {
            result.host = (char *) malloc(i * sizeof(char));
            strcpy(result.host, hostname);
            result.host[i] = '\0';
            if (c == ':')
            {
                result.is_port = true;
            }
            j++;
            free(hostname);
            break;
        }
        hostname[i] = c;
        i++;
        j++;
        if (i >= 20)
        {
            hostname_len+=20;
            hostname = (char *) realloc (hostname, (strlen(hostname)+1) * sizeof(char));
        }

    }
    i = 0;
    if (result.is_port) // get port
    {
        char port[6];
        while (1)
        {
            c = url[j];
            if ( c == '/' || c == '\0')
            {
                if (i < 1)
                {
                    result.error = true;
                    printf("Za dvojteckou nebyl zadan port\n");
                    return result;
                }
                result.port = (char *) malloc((strlen(port)+1) * sizeof(char));
                strcpy(result.port, port);
                result.port[strlen(port)] = '\0';
                j++;
                break;
            }
            if (c >= 48 && c <= 57 && i < 5)
            {
                port[i] = c;
                i++;
                j++;
            } else {
                result.error = true;
                printf("Chybny format portu\n");
                return result;
            }
            
        }
        
    } else { // if port not fount, set the default port
        if (result.is_https)
        {
            result.port = (char *) malloc(3 * sizeof(char));
            strcpy(result.port, "443");
            result.port[3] = '\0';
        } else {
            result.port = (char *) malloc(3 * sizeof(char));
            strcpy(result.port, "80");
            result.port[2] = '\0';
        }
        
    }
    int path_len = strlen(url) - j + 1;
    char path[path_len];
    path[0] = '/';
    i = 1;

    while (1) // get path
    {
        c = url[j];
        if (c == '\0')
        {
            result.path = (char *) malloc(i * sizeof(char));
            strcpy(result.path, path);
            result.path[i] = '\0';
            j++;
            break;
        }
        path[i] = url[j];
        i++;
        j++;
    }

    return result;
}

