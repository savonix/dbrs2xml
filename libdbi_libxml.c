/* <!--

Copyright:  Savonix
Author:     Albert Lash

--> */
#include <stdio.h>
#include <string.h>
#include <dbi/dbi.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

/*
http://xmlsoft.org/examples/tree2.c
*/

static void xmlencode_print(const char *src, unsigned int length);
static void print_table_data_xml(dbi_result *result, char *query_name);

static const char *xmlmeta[] = {
  "&", "&amp;",
  "<", "&lt;",
  ">", "&gt;",
  "\"", "&quot;",
  0, 0
};

int main(int argc, char *argv[])
{

    dbi_conn conn;
    dbi_result result;

    double threshold = 4.333333;
    unsigned int idnumber;
    const char *fullname;
    char my_query[1255];
    unsigned long myint;

    myint = 100000;

    dbi_initialize(NULL);
    conn = dbi_conn_new("mysql");

    #include "connection.c"

    if (dbi_conn_connect(conn) < 0) {
        printf("Could not connect. Please check the option settings\n");
    }
    else {
        sprintf(my_query, "SELECT * from bb_ib_forums WHERE forum_id < %i", myint);
        result = dbi_conn_queryf(conn, my_query, threshold);

        if (result) {
        print_table_data_xml(result,"forums_get_all");
        dbi_result_free(result);
        }
        dbi_conn_close(conn);
    }

}




static void
print_table_data_xml(dbi_result *result, char *query_name)
{
    dbi_result   cur;
    dbi_result *fields;
    char *myval;
    char *elt;
    unsigned long myint;
    char buffer[100];
    xmlDocPtr doc = NULL;
    xmlNodePtr root_node = NULL, node = NULL, top_query = NULL, query_row = NULL;
    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST "root");
    xmlDocSetRootElement(doc, root_node);
    top_query = xmlNewChild(root_node, NULL, BAD_CAST query_name,NULL);
    
    unsigned int i;
    while (dbi_result_next_row(result))
    {

        query_row = xmlNewChild(top_query, NULL, BAD_CAST elt,NULL);
        for (i=1; i < dbi_result_get_numfields(result); i++)
        {
            elt = strdup(dbi_result_get_field_name(result,i));
            if (dbi_result_get_field_type_idx(result,i) == 3) {
                if (dbi_result_get_string_idx(result,i))
                {
                    xmlNewChild(query_row, NULL, BAD_CAST elt,dbi_result_get_string_idx(result,i));
                }
            }
            if (dbi_result_get_field_type_idx(result,i) == 1) {
                if (dbi_result_get_ulonglong_idx(result,i))
                {
                    myint = dbi_result_get_ulonglong_idx(result,i);
                    sprintf(buffer, "%i", myint);
                    xmlNewChild(query_row, NULL, BAD_CAST elt, buffer);
                }
            }
        }

    }

    xmlSaveFormatFileEnc("-", doc, "UTF-8", 1);

    xmlFreeDoc(doc);

    xmlCleanupParser();

}
