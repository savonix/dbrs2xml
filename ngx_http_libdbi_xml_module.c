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

static char *ngx_http_libdbi_xml(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);

static ngx_command_t  ngx_http_libdbi_xml_commands[] = {

    { ngx_string("libdbi_xml"),
      NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS,
      ngx_http_libdbi_xml,
      0,
      0,
      NULL },

      ngx_null_command
};



static u_char  ngx_libdbi_xml[] = {

    'h'

};


static ngx_http_module_t  ngx_http_libdbi_xml_module_ctx = {
    NULL,                          /* preconfiguration */
    NULL,                          /* postconfiguration */

    NULL,                          /* create main configuration */
    NULL,                          /* init main configuration */

    NULL,                          /* create server configuration */
    NULL,                          /* merge server configuration */

    NULL,                          /* create location configuration */
    NULL                           /* merge location configuration */
};



/* Parse SQL file, prepare it */
/* It would be best to mimic the xslt filter module behavior, the SQL
string can either be from a file, or from a proxied source, or fastcgi */

/* gather parameters */
/* this could probably be taken care of by an xslt filter */

/* execute query */
/* return result set in xml */
/* this part has been prototyped in libdbi_xml.c */
