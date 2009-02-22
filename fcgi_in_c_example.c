#include <fcgi_stdio.h>
#include <fcgiapp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dbi/dbi.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <stdbool.h>
#include "qDecoder.h"

/*
 gcc -Wall -pedantic `xml2-config --cflags --libs` -I/usr/include  -L/usr/lib -lfcgi -lm -ldl -ldbi fcgi_in_c_example.c -o fcgi_in_c_example
*/

int i;
int qnum;
xmlDtdPtr config_dtd = NULL;
xmlDocPtr doc = NULL, sitemap_doc = NULL, config_doc = NULL;
xmlNodePtr root_node = NULL, sitemap_children = NULL, env_node = NULL, sql_node=NULL, get_node = NULL, post_node = NULL, cookie_node = NULL;
xmlChar *myxml = NULL, *sitemap_file = NULL;
int mylen;
char *qname;
char *qvalue;
Q_ENTRY *qget;
Q_ENTRY *req;
dbi_conn conn;
dbi_result result;
double threshold = 4.333333;
char *myq;
char *file;
char *config_file;
char *config_dtd_file;
static xmlNodePtr query_doc(dbi_result *result, char *query_name);
static char query_handler(char *qname, char *myq);
static char parse_query(char *file);
xmlXPathContextPtr xpathCtx;
xmlXPathObjectPtr xpathObj; 

void initialize(void)
{
    xmlInitParser();
    xmlSubstituteEntitiesDefault(1);
    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST "_R_");
    xmlDocSetRootElement(doc, root_node);
}

int main(void)
{
    int size;
    char buffer[100];
    initialize();

    while (FCGI_Accept() >= 0)   {
        if (config_doc == NULL) {
            config_doc = xmlParseFile(getenv("CONFIG_FILE"));
            xmlAddChild(root_node,xmlDocGetRootElement(config_doc));
            xpathCtx = xmlXPathNewContext(config_doc);
            xpathObj = xmlXPathEvalExpression(BAD_CAST "//sitemap", xpathCtx);
            size = xpathObj->nodesetval->nodeNr;
            sitemap_children = xpathObj->nodesetval->nodeTab[0]->children;
            sitemap_file = sitemap_children->content;
            if(sitemap_file == NULL) {
                sitemap_file = "nothing";
            }
            sprintf(buffer, "%s", sitemap_file);
            xmlNewChild(root_node, NULL, BAD_CAST "sitemap", BAD_CAST buffer);

        }

        /* Environment variables */
        env_node = xmlNewChild(root_node, NULL, BAD_CAST "ENV", NULL);
        xmlNewChild(env_node, NULL, BAD_CAST "SERVER_HOSTNAME", BAD_CAST getenv("SERVER_HOSTNAME"));
        xmlNewChild(env_node, NULL, BAD_CAST "SCRIPT_FILENAME", BAD_CAST getenv("SCRIPT_FILENAME"));
        xmlNewChild(env_node, NULL, BAD_CAST "QUERY_STRING", BAD_CAST qEncodeUrl(getenv("SCRIPT_FILENAME")));
        xmlNewChild(env_node, NULL, BAD_CAST "REQUEST_METHOD", BAD_CAST getenv("REQUEST_METHOD"));
        xmlNewChild(env_node, NULL, BAD_CAST "CONTENT_TYPE", BAD_CAST getenv("CONTENT_TYPE"));
        xmlNewChild(env_node, NULL, BAD_CAST "CONTENT_LENGTH", BAD_CAST getenv("CONTENT_LENGTH"));

        /* Get variables */
        get_node = xmlNewChild(root_node, NULL, BAD_CAST "GET", NULL);
        /* Parse queries. */
        qget = qCgiRequestParseQueries(NULL, "GET");
        qnum = qEntryGetNum(qget);
        /* Create children */
        for(i=0; i<qnum; i++) {
            qname = (char *)qEntryGetName(qget);
            qvalue = (char *)qEntryGetValue(qget);
            xmlNewChild(get_node, NULL, BAD_CAST qname, BAD_CAST qvalue);
        }


        /* Post variables */
        post_node = xmlNewChild(root_node, NULL, BAD_CAST "POST", NULL);
        /* Parse queries. */
        qget = qCgiRequestParseQueries(NULL, "POST");
        qnum = qEntryGetNum(qget);
        /* Create children */
        for(i=0; i<qnum; i++) {
            qname = (char *)qEntryGetName(qget);
            qvalue = (char *)qEntryGetValue(qget);
            xmlNewChild(post_node, NULL, BAD_CAST qname, BAD_CAST qvalue);
        }

        /* Cookie variables */
        cookie_node = xmlNewChild(root_node, NULL, BAD_CAST "COOKIE", NULL);
        /* Parse queries. */
        qget = qCgiRequestParseCookies(NULL);
        qnum = qEntryGetNum(qget);
        /* Create children */
        for(i=0; i<qnum; i++) {
            qname = (char *)qEntryGetName(qget);
            qvalue = (char *)qEntryGetValue(qget);
            xmlNewChild(cookie_node, NULL, BAD_CAST qname, BAD_CAST qvalue);
        }





        sql_node = xmlNewChild(root_node, NULL, BAD_CAST "SQL", NULL);











        /* Output XML */
        mylen = xmlStrlen(myxml);
        xmlDocDumpMemoryEnc(doc, &myxml, &mylen, "UTF-8");
        printf("Content-type: text/xml\r\n\r\n%s",myxml);
        /* Free everything under the root */
        xmlUnlinkNode(env_node);
        xmlFreeNode(env_node);
        xmlUnlinkNode(get_node);
        xmlFreeNode(get_node);
        xmlUnlinkNode(post_node);
        xmlFreeNode(post_node);
        xmlUnlinkNode(cookie_node);
        xmlFreeNode(cookie_node);
        xmlUnlinkNode(sql_node);
        xmlFreeNode(sql_node);
    }

    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 0;
}



static char
query_handler(char *qname, char *myq)
{

    if(myq == NULL) {
        myq = "SELECT NULL";
    }
    if(qname == NULL) {
        qname = "noname";
    }
    dbi_initialize(NULL);
    conn = dbi_conn_new("mysql");
    #include "connection.c"
    if (dbi_conn_connect(conn) < 0) {
        printf("Could not connect. Please check the option settings and if the" \
            "specific driver is available\n");
    } else {
        result = dbi_conn_queryf(conn, myq, threshold);
        if (result) {
        xmlAddChild(sql_node,query_doc(result,qname));
        dbi_result_free(result);
        }
        dbi_conn_close(conn);
    }
    return 0;

}

static char
parse_query(char *file)
{



    return 0;

}


static xmlNodePtr
query_doc(dbi_result *result, char *query_name)
{
    char *elt;
    unsigned long mylong;
    char buffer[100];
    unsigned int i;

    xmlDocPtr doc = NULL;
    xmlNodePtr top_query = NULL, query_row = NULL;
    doc = xmlNewDoc(BAD_CAST "1.0");
    top_query = xmlNewNode(NULL, BAD_CAST query_name);
    xmlDocSetRootElement(doc, top_query);

    while (dbi_result_next_row(result))
    {

        query_row = xmlNewChild(top_query, NULL, BAD_CAST query_name,NULL);
        for (i=1; i < dbi_result_get_numfields(result) + 1; i++)
        {
            elt = strdup(dbi_result_get_field_name(result,i));
            if (dbi_result_get_field_type_idx(result,i) == 3) {
                if (dbi_result_get_string_idx(result,i))
                {
                    xmlNewChild(query_row, NULL, BAD_CAST elt, BAD_CAST dbi_result_get_string_idx(result,i));
                }
            }
            if (dbi_result_get_field_type_idx(result,i) == 1) {
                if (dbi_result_get_ulonglong_idx(result,i))
                {
                    mylong = dbi_result_get_ulonglong_idx(result,i);
                    sprintf(buffer, "%ld", mylong);
                    xmlNewChild(query_row, NULL, BAD_CAST elt, BAD_CAST buffer);
                }
            }
        }

    }

    return xmlDocGetRootElement(doc);

    xmlFreeDoc(doc);
    xmlCleanupParser();

}
