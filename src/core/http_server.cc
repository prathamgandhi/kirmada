#include <iostream>
#include <sstream>
#include <string>

#include <core/http_server.hh>

Kirmada::HttpServer::HttpServer(int port, const char *host)
{
    socket = new HttpSocket(port, host);
}

Kirmada::HttpServer::~HttpServer()
{
    delete socket;
}

void Kirmada::HttpServer::start()
{
    for ( ;; ) {
        socket->acceptConnection();
        std::string data;
        socket->recvData(data);
        HttpRequest httpRequest = parseHttpRequest(data);
        std::string response = httpRequest.protocol + " 200 OK \r\n\r\nRequested path: " + httpRequest.path + "\r\n";
        socket->sendData(response);
        std::cout << response << std::endl;
        socket->closeConnection();
    }
}

const Kirmada::HttpServer::HttpRequest Kirmada::HttpServer::parseHttpRequest(std::string &requestBody) {
    HttpRequest parsedHttpRequest;
    std::istringstream requestStream(requestBody);
    std::string line;
    getline(requestStream, line);
    std::istringstream lineStream(line);
    lineStream >> parsedHttpRequest.method >> parsedHttpRequest.path >> parsedHttpRequest.protocol; 
    while (getline(requestStream, line) && line != "\r") {
        size_t delimiterPos = line.find(':');
        if (delimiterPos != std::string::npos) {
            std::string key = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 2); // Skip ': ' after the key
            parsedHttpRequest.headers[key] = value;
        }
    }
    return parsedHttpRequest;
}

