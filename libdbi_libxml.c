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

static void xmlencode_print(const char *src, uint length);
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

    dbi_initialize(NULL);
    conn = dbi_conn_new("mysql");

    #include "connection.c"

    if (dbi_conn_connect(conn) < 0) {
      printf("Could not connect. Please check the option settings\n");
    }
    else {
      result = dbi_conn_queryf(conn, "SELECT * from bb_ib_forums", threshold);

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
  ulong myint;
  xmlDocPtr doc = NULL;
  xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;
    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST "root");
    xmlDocSetRootElement(doc, root_node);
    top_query = xmlNewChild(root_node, NULL, BAD_CAST query_name,NULL);
    dbi_result_get_field_name(result,0);

  uint i;
  while (dbi_result_next_row(result))
  {
    xmlNewChild(top_query, NULL, BAD_CAST query_name,NULL);
    for (i=1; i < dbi_result_get_numfields(result); i++)
    {
        if (dbi_result_get_field_type_idx(result,i) == 3) {
            if (dbi_result_get_string_idx(result,i))
            {
                xmlNewChild(query_row, NULL, BAD_CAST dbi_result_get_field_name(result,i),dbi_result_get_string_idx(result,i));
            }
        }
        if (dbi_result_get_field_type_idx(result,i) == 1) {
            if (dbi_result_get_ulonglong_idx(result,i))
            {
                xmlNewChild(query_row, NULL, BAD_CAST dbi_result_get_field_name(result,i),dbi_result_get_ulonglong_idx(result,i));
            }
        }
    }

  }

    xmlSaveFormatFileEnc(argc > 1 ? argv[1] : "-", doc, "UTF-8", 1);
    
    /*free the document */
    xmlFreeDoc(doc);
    
    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();
}


static const char *array_value(const char **array, char key)
{
  for (; *array; array+= 2)
    if (**array == key)
      return array[1];
  return 0;
}


static void
xmlencode_print(const char *src, uint length)
{
  if (!src)
    printf("NULL");
  else
  {
    const char *p;
    for (p = src; *p && length; *p++, length--)
    {
      const char *t;
      if ((t = array_value(xmlmeta, *p))) {
          printf(t);
      } else {
          printf("%c",*p);
      }
    }
  }
}
