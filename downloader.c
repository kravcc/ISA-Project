#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/crypto.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "downloader.h"
#include "url_parser.h"

#define BuffSize 1024

BIO *bio;
SSL_CTX *ctx;
SSL *ssl;
char *certfile = NULL;
char *certaddr = NULL;

void init_ssl()
{
    
}

void cleanup(SSL_CTX* ctx, BIO* bio) 
{
    SSL_CTX_free(ctx);
    BIO_free_all(bio);
}

void report_and_exit(const char* msg) 
{
    perror(msg);
    ERR_print_errors_fp(stderr);
}

struct down_content get_dw_content() 
{
    struct down_content result;
    result.content = NULL;
    result.error = false;
    return result;
}

void free_dw_content(struct down_content *content) 
{
    if (content->content != NULL) {
        free(content->content);
        content->content = NULL;
    }
}

void set_certificates(char *certfile_path, char* certadress_path)
{
    if (certfile_path)
    {
        certfile = certfile_path;
    }
    if (certadress_path)
    {
        certaddr = certadress_path;
    }
}

struct down_content secure_connect(char* url) {
    SSL_load_error_strings();
	ERR_load_BIO_strings();
	OpenSSL_add_all_algorithms();
	SSL_library_init();
    char request[BuffSize*128];

    struct down_content result = get_dw_content();
    
    struct url_content parsed_url = parse_url(url);
    if (parsed_url.error)
    {
        free_url_content(&parsed_url);
        result.error = true;
        return result;
    }
    
    char hostname[20];
    for (int i = 0; i < 20; i++)
    {
        hostname[i] = '\0';
    }

    
    strcat(hostname, parsed_url.host);
    strcat(hostname, ":");
    strcat(hostname, parsed_url.port);
    if (!parsed_url.is_https)
    {
        bio = BIO_new_connect(hostname);
        if (NULL == bio){
            report_and_exit("BIO_new_ssl_connect...");
            result.error = true;
            return result;
        } 

        if (BIO_do_connect(bio) <= 0) {
            cleanup(ctx, bio);
            report_and_exit("BIO_do_connect...");
            result.error = true;
            return result;
        }

    } else {
        ctx = SSL_CTX_new(SSLv23_client_method());
        if (NULL == ctx){
            report_and_exit("SSL_CTX_new...");
            result.error = true;
            return result;
        } 

        if (certaddr)
        {
            printf("certadr %s\n", certaddr);
            if (!SSL_CTX_load_verify_locations(ctx, NULL, certaddr))
            {
                printf("Chyba: nepodařilo se ověřit platnost certifikátu\n");
            }
        }
        if (certfile)
        {
            printf("certfile %s\n", certfile);
            if (!SSL_CTX_load_verify_locations(ctx, certfile, NULL))
            {
                printf("Chyba: nepodařilo se ověřit platnost certifikátu\n");
            }
             
        }
        if (certaddr == NULL && certfile == NULL)
        {
            SSL_CTX_set_default_verify_paths(ctx);
        }
        
        bio = BIO_new_ssl_connect(ctx);
        if (NULL == bio) {
            result.error = true;
            return result;
            report_and_exit("BIO_new_ssl_connect...");
        }

        ssl = NULL;


        BIO_get_ssl(bio, &ssl); /* session */
        SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY); /* reliability */
        BIO_set_conn_hostname(bio, hostname); /* connection preparation */

        /* trying to connect */
        
        if (BIO_do_connect(bio) <= 0) {
            printf("Error %ld\n", BIO_do_connect(bio));
            cleanup(ctx, bio);
            report_and_exit("BIO_do_connect........");
            result.error = true;
            return result;
        }


        long verify_flag = SSL_get_verify_result(ssl);
        if (verify_flag != X509_V_OK){
            fprintf(stderr,
                    "##### Certificate verification error (%i) but continuing...\n",
                    (int) verify_flag);
            result.error = true;
            return result;
        }
            
    }
    memset(request, '\0', sizeof(request));
    sprintf(request,"GET %s HTTP/1.0\r\nHost: %s\r\nAccept: */*\r\nConnection: Close\r\n\r\n",
            parsed_url.path, parsed_url.host);

    //printf("-----REQUEST IS-----\n %s\n", request);
    
    bool first_write = true, write_success = false;
    while(first_write || BIO_should_retry(bio)){
        first_write = false;
			if (BIO_write(bio, request, strlen(request)))
			{
				write_success = true;
				break;
			}
    }
    if (!write_success)
    {
        report_and_exit("Bio write error\n");
        result.error = true;
        return result;
    }
    

    int buffer_size = 1024*128;
    int buffer_increment = 1024*128;
    int buffer_used = 0;
    char *response = (char *) malloc(buffer_size+1);
    while(42) {
        int len = BIO_read(bio, response+buffer_used, buffer_size-buffer_used);

        if (len < 0) {
            if (!BIO_should_retry(bio)) {
                report_and_exit("Bio read error\n");
                result.error = true;
                return result;
            } else {
                continue;
            }
        } else if (len == 0) {
            break;
        } else {
            buffer_used += len;
            if (buffer_size-buffer_used <= buffer_increment/2) {
                buffer_size += buffer_increment;
                response = (char *) realloc(response, buffer_size+1);
            }
            continue;
        }
    }
    response[buffer_used] = '\0';

    cleanup(ctx, bio);

    int i = 0;
    bool http_code = false;
    while (1)
    {
        if (response[i] == ' ')
        {
            http_code = true;
            i++;
            continue;
        }
        if (http_code)
        {
            if (response[i] != '2')
            {
                printf("HTTP_DOWNLOADER - Vysledny HTTP kod neni 200 (OK) - misto toho je %c%c%c\n", response[i], response[i+1], response[i+2]);
                result.error = true;
                free(response);
                return result;
            }
            break;
        }
        i++;
    }
    result.content = response;
    
    free_url_content(&parsed_url);
    return result;
}
