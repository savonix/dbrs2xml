#include <fcgi_stdio.h>
#include <fcgiapp.h>
#include <stdlib.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <stdbool.h>
/*
#include "qDecoder.h"
*/
/*
 gcc -Wall -pedantic `xml2-config --cflags --libs` -I/usr/include  -L/usr/lib -lfcgi -lm -ldl -ldbi fcgi_in_c_example.c -o fcgi_in_c_example
*/

int count;
xmlDocPtr doc = NULL;
xmlNodePtr root_node = NULL, env_node = NULL, get_node = NULL, post_node = NULL;
xmlChar *myxml = NULL;
int mylen;

void initialize(void)
{
    count=0;
    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST "_R_");
    xmlDocSetRootElement(doc, root_node);
}

int main()
{

    initialize();

    while (FCGI_Accept() >= 0)   {
        /* Environment variables */
        env_node = xmlNewChild(root_node, NULL, BAD_CAST "_env", NULL);
        xmlNewChild(env_node, NULL, BAD_CAST "SERVER_HOSTNAME", BAD_CAST getenv("SERVER_HOSTNAME"));
        xmlNewChild(env_node, NULL, BAD_CAST "SCRIPT_FILENAME", BAD_CAST getenv("SCRIPT_FILENAME"));
        xmlNewChild(env_node, NULL, BAD_CAST "QUERY_STRING", BAD_CAST getenv("QUERY_STRING"));
        xmlNewChild(env_node, NULL, BAD_CAST "REQUEST_METHOD", BAD_CAST getenv("REQUEST_METHOD"));
        xmlNewChild(env_node, NULL, BAD_CAST "CONTENT_TYPE", BAD_CAST getenv("CONTENT_TYPE"));
        xmlNewChild(env_node, NULL, BAD_CAST "CONTENT_LENGTH", BAD_CAST getenv("CONTENT_LENGTH"));


        /* Get variables */
        get_node = xmlNewChild(root_node, NULL, BAD_CAST "_get", NULL);

        /* Post variables */
        post_node = xmlNewChild(root_node, NULL, BAD_CAST "_post", NULL);

        /* Output XML */
        mylen = xmlStrlen(myxml);
        xmlDocDumpMemoryEnc(doc, &myxml, &mylen, "UTF-8");
        printf("Content-type: text/xml\r\n"
               "\r\n"
               "%s",myxml);
        xmlUnlinkNode(env_node);
        xmlFreeNode(env_node);
        xmlUnlinkNode(get_node);
        xmlFreeNode(get_node);
        xmlUnlinkNode(post_node);
        xmlFreeNode(post_node);
        /* xmlSaveFormatFileEnc("-", doc, "UTF-8", 1); */
    }

    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 0;
}
