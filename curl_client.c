/* <!--

Copyright:  Savonix
Author:     Albert Lash

--> */
#include <fcgi_stdio.h>
#include <fcgiapp.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>


size_t  write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
  char buf[size*nmemb+1];
  char * pbuf = &buf[0];
  memset(buf, '\0', size*nmemb+1);
  size_t i = 0;
  for(;  i < nmemb ; i++){
    strncpy(pbuf,ptr,size);
    pbuf += size;
    ptr += size;
  }
  printf("\r\n\r\n%s",buf);
  return size * nmemb;
}


int main(void)
{
  CURL *curl;
  CURLcode res;

    while (FCGI_Accept() >= 0)   {

        curl = curl_easy_init();

        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, "www.sf-notes.com");
            curl_easy_setopt(curl,   CURLOPT_NOPROGRESS  ,1);
            curl_easy_setopt(curl,   CURLOPT_WRITEFUNCTION, &write_data);

            res = curl_easy_perform(curl);

            curl_easy_cleanup(curl);
        }
    }
    return 0;
}

