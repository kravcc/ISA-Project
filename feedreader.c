#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "args_parser.h"
#include "feed_parser.h"
#include "url_parser.h"
#include "xml_parser.h"
#include "downloader.h"

#define STATUS_OK       0 // OK
#define STATUS_ERROR   1 // Error

struct par_content result;
struct feed_content feedfile;
struct url_content result_url;
struct down_content downHTTP;
struct xml_content xml;


void print_help() {
    printf(
        "--- CTECKA FEEDU RSS A ATOM ---\n"
        "--- Autor: MAREK SIPOS (xsipos03) ---\n"
        "\n"
        "Pouziti: feedreader <URL | -f <feedfile>> [-c <certfile>] [-C <certaddr>] [-T] [-a] [-u] [-h]\n"
        "\n"
        "URL | -f <feedfile>\n"
        "Povinny parametr, slouzi k urceni adresy URL jednoho feedu nebo souboru s adresami URL pro vice feedu.\n"
        "Soubor feedfile umoznuje psani komentaru uvozenych znakem # az do konce daneho radku.\n"
        "Je mozne zadat najednou bud jednu adresu URL, nebo jeden soubor s feedy.\n"
        "\n"
        "-c <certfile>\n"
        "Volitelny parametr, slouzi k zadani souboru s duveryhodnymi certifikaty pro zabezpecenou komunikaci (HTTPS).\n"
        "Pokud nejsou parametry -c a -C pouzity ani jednou, k vyhledani certifikatu se pouzije vychozi uloziste v systemu.\n"
        "\n"
        "-C <certaddr>\n"
        "Volitelny parametr, slouzi k zadani slozky s duveryhodnymi certifikaty pro zabezpecenou komunikaci (HTTPS).\n"
        "Pokud nejsou parametry -c a -C pouzity ani jednou, k vyhledani certifikatu se pouzije vychozi uloziste v systemu.\n"
        "\n"
        "-T\n"
        "Volitelny parametr, ve vysledcich dojde k zahrnuti casu zmeny ci vytvoreni zaznamu.\n"
        "\n"
        "-a\n"
        "Volitelny parametr, ve vysledcich dojde k zahrnuti identifikacnich udaju autora zaznamu.\n"
        "\n"
        "-u\n"
        "Volitelny parametr, ve vysledcich dojde k zahrnuti asociovane adresy URL zaznamu.\n"
        "\n"
        "-h\n"
        "Volitelny parametr, pokud je pri volani programu zadan samostatne, dojde k vyvolani teto napovedy.\n"
        "------------------------\n"
    );
}

void end_program(int status)
{
    free_par_content(&result);
    free_dw_content(&downHTTP);
    exit(status);
}

int main(int argc, char *argv[])
{
    result = par_init();
    // parsing of arguments
    result = parser(argc, argv); 
    if (result.error)
    {
        end_program(STATUS_ERROR);
    }
    // init structurs
    result_url = url_init();
    feedfile = feed_init();
    
    if (result.is_help)
    {
        print_help();
        end_program(STATUS_OK);
    }
    if (result.feedfile == NULL && result.url == NULL)
    {
        printf("Nebyl zadan zadny url zdroj\n");
        end_program(STATUS_ERROR);
    }
    
    // feedfile parsing
    if (result.feedfile != NULL)
    {
        feedfile = parse_feedfile(result.feedfile);
        if (feedfile.error)
        {
            end_program(STATUS_ERROR);
        }
    }
    // set certificates
    if (result.certaddr)
    {
        set_certificates(NULL, result.certaddr);
    }
    if (result.certfile)
    {
        set_certificates(result.certfile, NULL);
    }
    
    for (int i = 0; i < (result.url ? 1 : feedfile.feed_counter); i++) {
        //printf("%s\n", hostname);
        downHTTP = secure_connect(result.url ? result.url : feedfile.feeds[i]);
        if (downHTTP.error)
        {
            end_program(STATUS_ERROR);
        }

        char *istr;
        istr = strstr (downHTTP.content, "\r\n\r\n"); // separating the header from the xml file
        xml = get_xml_content();
        xml = parse_xml(istr+4); // start parsing xml
        if (xml.error)
        {
            end_program(STATUS_ERROR);
        }

        // print the final content
        printf("*** %s ***\n", xml.name == NULL ? "<Feed bez nazvu>" : xml.name);
            for (int j = 0; j < xml.num_items; j++) {
                printf("%s\n", xml.items[j].name == NULL ? "<Zaznam bez nazvu>" : xml.items[j].name);
                if (result.is_t) {
                    printf("Aktualizace: %s\n", xml.items[j].time == NULL ? "<Datum neurceno>" : xml.items[j].time);
                }
                if (result.is_a) {
                    printf("Autor: %s\n", xml.items[j].author == NULL ? "<Autor neurcen>" : xml.items[j].author);
                }
                if (result.is_u) {
                    printf("URL: %s\n", xml.items[j].url == NULL ? "<URL neurceno>" : xml.items[j].url);
                }
                if (result.is_t || result.is_a || result.is_u) {
                    if (j < xml.num_items-1){
                        printf("\n");
                    }
                }
            }
        if (result.feedfile)
        {
            if (i < feedfile.feed_counter - 1)
            {
                printf("\n");
            }   
        }
        // if(result.feedfile && feedfile.feed_counter > i){
        //free_xml_content(&xml);
        // }
    }
    free_dw_content(&downHTTP); // clear
    free_par_content(&result);
    return 0;
}