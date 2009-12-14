/* <!--

Copyright:  Savonix
Author:     Albert Lash

--> */
#include <fcgi_stdio.h>
#include <fcgiapp.h>
#include <stdio.h>
#include <string.h>


int main(void)
{

    while (FCGI_Accept() >= 0)   {
        printf("HTTP/1.1 301 Moved Permanently\r\n");
        printf("Location: http://%s\r\n\r\n",location);
    }
    return 0;
}

