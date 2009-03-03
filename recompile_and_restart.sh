#!/bin/sh
gcc `xml2-config --cflags --libs` -I/usr/local/include  -L/usr/local/lib -lqDecoder -lfcgi -lm -ldl -ldbi fcgi_in_c_example.c -o fcgi_in_c_example
killall fcgi_in_c_example && ./spawn-fcgi -s /var/run/dbrs2xml.sock fcgi_in_c_example && chmod 0666 /var/run/dbrs2xml.sock
