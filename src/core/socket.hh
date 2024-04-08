#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstddef>

namespace Kirmada {

class SocketBase {
    private:
        struct addrinfo hints;
    protected:
        int sockfd;
        struct addrinfo *servInfo;
    public:

        enum SocketType {
            TCP,
            UDP
        };

        SocketBase(int port, SocketType type = TCP, const char *host = NULL);
        ~SocketBase();
};

class ServerSocketBase : public SocketBase {

    private:
        struct sockaddr_storage incoming_addr;
        socklen_t addr_size = sizeof incoming_addr;
    protected:
        int conn_sockfd;
    public:
        ServerSocketBase(int port, SocketType type = TCP, const char *host = NULL);
        void acceptConnection();
        void closeConnection();
};

}