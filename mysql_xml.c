/* <!--

Copyright:  Savonix
Author:     Albert Lash

--> */
#include <stdio.h>
#include <string.h>
#include <my_global.h>
#include "mysql.h"
#include "mysql_version.h"
#include "mysqld_error.h"
#include <m_string.h>
#include <signal.h>

static void xmlencode_print(const char *src, uint length);
static void print_table_data_xml(MYSQL_RES *result, char *query_name);

static const char *xmlmeta[] = {
  "&", "&amp;",
  "<", "&lt;",
  ">", "&gt;",
  "\"", "&quot;",
  0, 0
};


int mysql_exec_sql(MYSQL *mysql,const char *create_definition)
{
   return mysql_real_query(mysql,create_definition,strlen(create_definition));
}

int main()

{

   MYSQL mysql;
char record[1000];
MYSQL_RES *result;
MYSQL_ROW row;

unsigned int num_fields;
unsigned int i;
char query_def[1000];

   mysql_init(&mysql);

   mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"blah");

    if (!mysql_real_connect(&mysql,"host","username","password","database",0,NULL,0))
    {
    } else {
    strmov(query_def,"SELECT * from bb_ib_forums");

    if(mysql_exec_sql(&mysql,query_def)==0)
    {
        result = mysql_store_result(&mysql);
    } else {
        printf( "Error" );
    }
    print_table_data_xml(result,"forums_get_all");
}
}




static void
print_table_data_xml(MYSQL_RES *result, char *query_name)
{
  MYSQL_ROW   cur;
  MYSQL_FIELD *fields;
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

