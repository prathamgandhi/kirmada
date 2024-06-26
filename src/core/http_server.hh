#include <core/http_socket.hh>
#include <unordered_map>

namespace Kirmada {
class HttpServer
{
    private:
        HttpSocket *socket;

        std::string serverRoot;

        class HttpRequest {
            public : 
            std::string method;
            std::string path;
            std::string protocol;
            std::unordered_map<std::string, std::string> headers;
        };
    public:
        HttpServer(int port, std::string serverRoot = "", const char *host = NULL);
        ~HttpServer();
        void start();
        const HttpRequest parseHttpRequest(std::string &requestBody);
        // friend ostream & operator << (ostream &out, const Complex &c);
};
}