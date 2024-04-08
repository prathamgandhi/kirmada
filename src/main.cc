#include <iostream>
#include "core/http_server.hh"

int main()
{
    Kirmada::HttpServer s(8008);
    s.start();
    return 0;    
}