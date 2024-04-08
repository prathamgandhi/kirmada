#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

#include <core/http_server.hh>

Kirmada::HttpServer::HttpServer(int port, std::string serverRoot, const char *host)
{
    socket = new HttpSocket(port, host);
    if (serverRoot == "") {
        serverRoot = "/home/pratham/www";
    }
    this->serverRoot = serverRoot;
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
        if (httpRequest.path == "/") {
            httpRequest.path = "/index.html";
        }
        std::string requestPath = serverRoot + httpRequest.path;
        std::ifstream requestFilePath(requestPath, std::ios::binary);
        if (!requestFilePath.is_open()) {
            fprintf(stderr, "Failed to open file : not found at %s\n", requestPath);
            std::string failedResponse = httpRequest.protocol + " 404 Not Found \r\n\r\n";
            socket->sendData(failedResponse);
            std::cout << failedResponse << std::endl;
            socket->closeConnection();
            continue;
        }
        std::stringstream fileBuffer;
        fileBuffer << requestFilePath.rdbuf();
        requestFilePath.close();
        std::string fileBufferString = fileBuffer.str();
        std::string response = httpRequest.protocol + " 200 OK \r\n\r\n" + fileBufferString;
        socket->sendData(response);
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

