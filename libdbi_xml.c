/* <!--

Copyright:  Savonix
Author:     Albert Lash

--> */
#include <stdio.h>
#include <string.h>
#include <dbi/dbi.h>


static void xmlencode_print(const char *src, uint length);
static void print_table_data_xml(RES *result, char *query_name);

static const char *xmlmeta[] = {
  "&", "&amp;",
  "<", "&lt;",
  ">", "&gt;",
  "\"", "&quot;",
  0, 0
};



int main()
{

    dbi_conn conn;
    dbi_result result;

    double threshold = 4.333333;
    unsigned int idnumber;
    const char *fullname;
    
    dbi_initialize(NULL);
    conn = dbi_conn_new("mysql");

    dbi_conn_set_option(conn, "host", "localhost");
    dbi_conn_set_option(conn, "username", "your_name");
    dbi_conn_set_option(conn, "password", "your_password");
    dbi_conn_set_option(conn, "dbname", "your_dbname");
    dbi_conn_set_option(conn, "encoding", "UTF-8");

    if (dbi_conn_connect(conn) < 0) {
      printf("Could not connect. Please check the option settings\n");
    }
    else {
      result = dbi_conn_queryf(conn, "SELECT * from bb_ib_forums", threshold);

      if (result) {

        dbi_result_free(result);
      }
      dbi_conn_close(conn);
    }

}




static void
print_table_data_xml(RES *result, char *query_name)
{
  ROW   cur;
  FIELD *fields;
  mysql_field_seek(result,0);
  printf("<?xml version=\"1.0\"?>\n");
  printf("<");
  printf(query_name);
  printf(">\n");
  fields = mysql_fetch_fields(result);
  uint i;
  while ((cur = mysql_fetch_row(result)))
  {
    ulong *lengths=mysql_fetch_lengths(result);
    printf("\t<");
    printf(query_name);
    printf(">\n");
    for (i=0; i < mysql_num_fields(result); i++)
    {
      printf("\t\t<");
      printf(fields[i].name, (uint) strlen(fields[i].name));
      if (cur[i])
      {
        printf(">");
        printf(cur[i], lengths[i]);
        printf("</");
        printf(fields[i].name, (uint) strlen(fields[i].name));
        printf(">\n");
      }
      else
        printf("\" xsi:nil=\"true\" />\n");
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
      if ((t = array_value(xmlmeta, *p)))
    printf(t);
      else
    printf(*p);
    }
  }
}
