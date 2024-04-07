#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <unistd.h>

#include <iostream>

#include "core/socket.hh"
#include "core/config.hh"

Kirmada::SocketBase::SocketBase(int port, SocketType type, const char *host)
{
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    switch (type) {
        case TCP:
            hints.ai_socktype = SOCK_STREAM;
            break;
        case UDP:
            hints.ai_socktype = SOCK_DGRAM;
            break;
    }
    hints.ai_flags = AI_PASSIVE;
    int status;
    const char *cPort = std::to_string(port).c_str();
    if ((status = getaddrinfo(host, cPort, &hints, &servInfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }
    for (; servInfo != NULL; servInfo = servInfo->ai_next) {
        sockfd = socket(servInfo->ai_family, servInfo->ai_socktype, servInfo->ai_protocol);
        if (!sockfd) {
            continue;
        }
        else {
            std::cout << "Socket initialized... " << sockfd << std::endl;
            break;
        }
    }
    if (!servInfo) {
        fprintf(stderr, "Error initializing socket: error code %d ('%s')\n", errno, strerror(errno));
        exit(1);
    }
}

Kirmada::SocketBase::~SocketBase()
{
    std::cout << "Closing socket " << sockfd << std::endl;
    close(sockfd);
    std::cout << "Cleaning servInfo" << std::endl;
    freeaddrinfo(servInfo);
}

Kirmada::ServerSocketBase::ServerSocketBase(int port, Kirmada::SocketBase::SocketType type, const char *host)
    : SocketBase(port, type, host) 
{
    int yes = 1;
    // This option allows reuse of ports once closed. Keeping this for now. To remove in production env.
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
        fprintf(stderr, "Error while setting socket option for reuse : error code %d ('%s')\n", errno, strerror(errno));
        exit(1);
    }

    bind(sockfd, servInfo->ai_addr, servInfo->ai_addrlen);


    if (listen(sockfd, LISTEN_BACKLOG) == -1) {
        fprintf(stderr, "Error while listening");
        exit(1);
    } 
}

void Kirmada::ServerSocketBase::acceptConnection()
{
    conn_sockfd = accept(sockfd, (struct sockaddr *)&incoming_addr, &addr_size);
}

Kirmada::ServerSocketBase::~ServerSocketBase()
{
    close(conn_sockfd);
}