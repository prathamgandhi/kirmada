#include "core/http_socket.hh"
#include "core/config.hh"

#include <cerrno>
#include <cstring>

Kirmada::HttpSocket::HttpSocket(int port, const char *host)
    : Kirmada::ServerSocketBase(port, Kirmada::SocketBase::TCP, host)
{
}

bool Kirmada::HttpSocket::sendData(std::string &data)
{
    ssize_t total_bytes_sent = 0;
    ssize_t bytes_sent = 0;
    const char *buffer = data.c_str();
    do {
        bytes_sent = send(conn_sockfd, buffer + total_bytes_sent, std::min(static_cast<size_t>(SEND_BUFFER_SIZE), static_cast<size_t>(data.length() - total_bytes_sent)), 0);
        if (bytes_sent == -1) {
            fprintf(stderr, "Error sending data over socket: error code %d ('%s')\n", errno, strerror(errno));
            return false;
        }
        total_bytes_sent += bytes_sent;
    } while (total_bytes_sent < data.length());
    return true;
}

bool Kirmada::HttpSocket::recvData(std::string &data)
{
    char recv_buffer[RECEIVER_BUFFER_SIZE];
    int total_bytes_received = 0;
    int bytes_received = 0;
    char *buffer = NULL;
    while(1) {
        bytes_received = recv(conn_sockfd, recv_buffer, RECEIVER_BUFFER_SIZE, 0);
        if (bytes_received == -1) {
            fprintf(stderr, "Error receiving data over socket: error code %d ('%s')\n", errno, strerror(errno));
            return false;
        }
        else if (bytes_received == 0) {
            break;
        }
        else {
            data.append(recv_buffer, bytes_received);
            if (data.back() == '\n') {
                break;
            }
        }
    }

    return false;
}