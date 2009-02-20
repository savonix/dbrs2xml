/*
    Author: Albert Lash
    Copyright: Savonix
*/
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <dbi/dbi.h>

/* Parse SQL file, prepare it */
/* It would be best to mimic the xslt filter module behavior, the SQL
string can either be from a file, or from a proxied source, or fastcgi */

/* gather parameters */
/* this could probably be taken care of by an xslt filter */

/* execute query */
/* return result set in xml */
/* this part has been prototyped in libdbi_xml.c */
