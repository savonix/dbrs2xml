/* <!--

Copyright:  Savonix
Author:     Albert Lash

--> */
#include <stdio.h>
#include <string.h>
#include <dbi/dbi.h>

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
    char my_query[1255];
    unsigned long myint;

    myint = 100000;
    dbi_initialize(NULL);
    conn = dbi_conn_new("mysql");

    #include "connection.c"

    if (dbi_conn_connect(conn) < 0) {
      printf("Could not connect. Please check the option settings\n");
    } else {

        sprintf(my_query, "SELECT * from bb_ib_forums WHERE forum_ib < %i", myint);
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
  ulong myint;
  dbi_result_get_field_name(result,0);
  printf("<?xml version=\"1.0\"?>\n");
  printf("<");
  printf(query_name);
  printf(">\n");
  uint i;
  while (dbi_result_next_row(result))
  {
    printf("\t<");
    printf(query_name);
    printf(">\n");
    for (i=1; i < dbi_result_get_numfields(result); i++)
    {
        printf("\t\t<");
        elt = strdup(dbi_result_get_field_name(result,i));
        printf("%s",elt,(uint) strlen(elt));
        printf(">");
        if (dbi_result_get_field_type_idx(result,i) == 3) {
            if (dbi_result_get_string_idx(result,i))
            {

                xmlencode_print(dbi_result_get_string_idx(result,i),dbi_result_get_field_length_idx(result,i));
            }
        }
        if (dbi_result_get_field_type_idx(result,i) == 1) {
            if (dbi_result_get_ulonglong_idx(result,i))
            {
                myint = dbi_result_get_ulonglong_idx(result,i);
                printf("%u",myint);
            }
        }
        printf("</");
        printf("%s",elt,(uint) strlen(elt));
        printf(">\n");
        free(elt);
    }
    (void) printf("\t</");
    (void) printf(query_name);
    (void) printf(">\n");

  }
  printf("</");
  printf(query_name);
  printf(">\n");

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
