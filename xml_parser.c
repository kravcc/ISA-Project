/**

ISA projekt - Čtečka novinek ve formátu Atom s podporou TLS
Autor: Marek Šipoš (xsipos03), 26.9. 2018

xml_parser.c - modul pro zpracování XML těla a extrakci potřebných dat

**/

#include <libxml/parser.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "xml_parser.h"

// parsing rss file
struct xml_content parse_xml_rss(xmlNodePtr xmlRoot)
{
    struct xml_content result = get_xml_content();
    for (xmlNodePtr rootNode = xmlRoot->children; rootNode; rootNode = rootNode->next)
	{
		if (!xmlStrcasecmp(rootNode->name, (xmlChar *) "channel"))
		{
			for (xmlNodePtr channelNode = rootNode->children; channelNode; channelNode = channelNode->next)
			{
				if (!xmlStrcasecmp(channelNode->name, (xmlChar *) "title")){
					// char *title = (char *) xmlNodeGetContent(channelNode);
                    // printf("title is %s\n", title);
                    result.name = (char*) malloc(strlen((char *) xmlNodeGetContent(channelNode)));
                    strcpy(result.name, (char *) xmlNodeGetContent(channelNode));
					break;
				}
			}
			break;
		}
	}
    int i = 0;
    result.items = (struct xml_item*) malloc(sizeof(struct xml_item));
    result.items[i] = get_xml_item();
    for (xmlNodePtr rootNode = xmlRoot->children; rootNode; rootNode = rootNode->next)
	{
		if (!xmlStrcasecmp(rootNode->name, (xmlChar *) "channel"))
		{
			for (xmlNodePtr channelNode = rootNode->children; channelNode; channelNode = channelNode->next)
			{
				if (!xmlStrcasecmp(channelNode->name, (xmlChar *) "item"))
				{
					for (xmlNodePtr itemNode = channelNode->children; itemNode; itemNode = itemNode->next)
                    {

                        if (!xmlStrcasecmp(itemNode->name, (xmlChar *) "title"))
						{
							// char * itemTitle = (char *) xmlNodeGetContent(itemNode);
                            // printf("itemtitle is %s\n", itemTitle);
                            result.items[i].name = (char*) malloc(strlen((char *) xmlNodeGetContent(itemNode)));
                            strcpy (result.items[i].name, (char *) xmlNodeGetContent(itemNode)); 
						}
                        if (!xmlStrcasecmp(itemNode->name,(xmlChar *) "pubDate"))
	                    {
                            result.items[i].time = (char*) malloc(strlen((char *) xmlNodeGetContent(itemNode)));
                            strcpy (result.items[i].time, (char *) xmlNodeGetContent(itemNode)); 
						}
						if (!xmlStrcasecmp(itemNode->name,(xmlChar *) "author"))
						{
							result.items[i].author = (char*) malloc(strlen((char *) xmlNodeGetContent(itemNode)));
                            strcpy (result.items[i].author, (char *) xmlNodeGetContent(itemNode)); 
						}
						if (!xmlStrcasecmp(itemNode->name,(xmlChar *) "link"))
						{
							result.items[i].url = (char*) malloc(strlen((char *) xmlNodeGetContent(itemNode)));
                            strcpy (result.items[i].url, (char *) xmlNodeGetContent(itemNode)); 
						}

                        
                    }
                    if (result.items[i].name != NULL)
                    {
                        i++;
                        result.items = (struct xml_item*) realloc(result.items, (i+1) * sizeof(struct xml_item));
                        result.items[i] = get_xml_item();
                    }
				}
			}
			break;
		}
	}
    result.num_items = i;
    return result;
}

// parsing atom file
struct xml_content parse_xml_atom(xmlNodePtr xmlRoot)
{
    struct xml_content result = get_xml_content();
    for (xmlNodePtr rootNode = xmlRoot->children; rootNode; rootNode = rootNode->next)
	{
		if (!xmlStrcasecmp(rootNode->name, (xmlChar *) "title"))
		{
            result.name = (char*) malloc(strlen((char *) xmlNodeGetContent(rootNode)));
            strcpy(result.name, (char *) xmlNodeGetContent(rootNode));
			break;
		}
	}
    int i = 0;
    result.items = malloc(sizeof(struct xml_item));
    
    for (xmlNodePtr rootNode = xmlRoot->children; rootNode; rootNode = rootNode->next)
	{
        if (!xmlStrcasecmp(rootNode->name, (xmlChar *) "entry"))
        {
            result.items[i] = get_xml_item();
            for (xmlNodePtr itemNode = rootNode->children; itemNode; itemNode = itemNode->next)
            {

                if (!xmlStrcasecmp(itemNode->name, (xmlChar *) "title"))
                {
                    // char * itemTitle = (char *) xmlNodeGetContent(itemNode);
                    //printf("itemtitle is %s\n", (char *) xmlNodeGetContent(itemNode));
                    result.items[i].name = (char*) malloc(strlen((char *) xmlNodeGetContent(itemNode)));
                    strcpy (result.items[i].name, (char *) xmlNodeGetContent(itemNode)); 
                }
                if (!xmlStrcasecmp(itemNode->name,(xmlChar *) "updated"))
                {
                    result.items[i].time = (char*) malloc(strlen((char *) xmlNodeGetContent(itemNode)));
                    strcpy (result.items[i].time, (char *) xmlNodeGetContent(itemNode)); 
                }
                if (!xmlStrcasecmp(itemNode->name,(xmlChar *) "author"))
                {
                    for (xmlNodePtr authorNode = itemNode->children; authorNode; authorNode = authorNode->next)
					{
						if (!xmlStrcasecmp(authorNode->name, (xmlChar *) "name") || !xmlStrcasecmp(authorNode->name, (xmlChar *) "email") )
						{
                            result.items[i].author = (char*) malloc(strlen((char *) xmlNodeGetContent(authorNode)));
                            strcpy (result.items[i].author, (char *) xmlNodeGetContent(authorNode)); 
						}
					}

                }
                if (!xmlStrcasecmp(itemNode->name,(xmlChar *) "link"))
                {
                    result.items[i].url = (char*) malloc(strlen((char *) xmlGetProp(itemNode, (xmlChar *) "href")));
                    strcpy (result.items[i].url, (char *) xmlGetProp(itemNode, (xmlChar *) "href")); 
                }

                
            }
            // if (result.items[i].name != NULL)
            // {
                i++;
                result.items = realloc(result.items, (i+1) * sizeof(struct xml_item));
                result.items[i] = get_xml_item();
            //}
        }
	}
    result.num_items = i;
    return result;
}


struct xml_content parse_xml(char *xml) {
    
    struct xml_content result = get_xml_content();

    // Check for empty XML
    if (strlen(xml) == 0) {
        printf("Xml is empty\n");
        result.error = true;
        return result;
    }

    // Init
    xmlDocPtr xmlDoc;
    xmlNodePtr xmlRoot;
    xmlDoc = xmlReadMemory(xml, strlen(xml), NULL, NULL, XML_PARSE_NOERROR | XML_PARSE_NOWARNING);

    // Convert to XML structure
    if (xmlDoc == NULL) {
        printf("Convert to XML structure failed\n");
        result.error = true;
        xmlCleanupParser();
        return result;
    }

    // Get root
    xmlRoot = xmlDocGetRootElement(xmlDoc);
    if (xmlRoot == NULL) {
        printf("Get root failed\n");
        result.error = true;
        xmlFreeDoc(xmlDoc);
        xmlCleanupParser();
        return result;
    }

    if(!strcmp((char *)xmlRoot->name, "rss")){
        result = parse_xml_rss(xmlRoot);
        xmlFreeDoc (xmlDoc); 
        xmlCleanupParser (); 
        return result;
    }

    
    result = parse_xml_atom(xmlRoot);
    xmlFreeDoc (xmlDoc); 
    xmlCleanupParser (); 
    return result;
}

struct xml_item get_xml_item() {
    struct xml_item result;
    result.name = NULL;
    result.time = NULL;
    result.author = NULL;
    result.url = NULL;
    return result;
}

void free_xml_item(struct xml_item *item) {
    if (item->name != NULL) {
        free(item->name);
        item->name = NULL;
    }
    if (item->time != NULL) {
        free(item->time);
        item->time = NULL;
    }
    if (item->author != NULL) {
        free(item->author);
        item->author = NULL;
    }
    if (item->url != NULL) {
        free(item->url);
        item->url = NULL;
    }
}

struct xml_content get_xml_content() {
    struct xml_content result;
    result.name = NULL;
    result.num_items = 0;
    result.items = NULL;
    result.error = false;
    return result;
}

void free_xml_content(struct xml_content *content) {
    if (content->name != NULL) {
        free(content->name);
        content->name = NULL;
    }
    if (content->num_items > 0) {
        
        for (int i = 0; i < content->num_items; i++) {
            free_xml_item(&content->items[i]);
        }
        free(content->items);
        content->items = NULL;
        content->num_items = 0;
    }
}
