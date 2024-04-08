#include <iostream>
#include "core/http_server.hh"

int main()
{
    Kirmada::HttpServer s(8008, "/home/pratham/www");
    s.start();
    return 0;    
}