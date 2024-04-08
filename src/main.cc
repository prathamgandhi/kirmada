#include <iostream>
#include "core/http_socket.hh"

int main()
{
    Kirmada::HttpSocket s(3008);
    s.acceptConnection();
    std::string data;
    s.recvData(data);
    std::cout << data;
    return 0;    
}