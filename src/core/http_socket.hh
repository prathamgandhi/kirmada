#include "core/socket.hh"
#include <string>

namespace Kirmada {
class HttpSocket : public ServerSocketBase{
private:

public:
    HttpSocket(int port, const char *host = NULL);
    bool sendData(std::string &data);
    bool recvData(std::string &data);
};
}