#include <iostream>
#include "core/socket.hh"

int main()
{
    Kirmada::ServerSocketBase s(3008);
    s.acceptConnection();
    s.sendData();
    return 0;    
}