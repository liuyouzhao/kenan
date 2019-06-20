#ifndef DCSOCKET_H
#define DCSOCKET_H

#include <sys/socket.h>
#include <vector>
#include <string>
#include <arpa/inet.h>
#include <unistd.h>

typedef void* (*CALLBACK)(void*, int, void* ptr);
#define  BUFFSIZE     1024

namespace DCanvas
{

class DCSocket
{
public:
    static    DCSocket* create();
    static    DCSocket* create(char* url);
    ~DCSocket();

    static bool parse(const char *url, int len, std::string& method, std::string& host, int &port, std::string& path, std::string& param);
    char* recieve(char* path);
    void  setPort(int port) { m_port = port; }
    void  setUrl(char* url);//host
    void  setAddress(char* ip) { m_host = ip; }
    void  setCallBack(CALLBACK p) { m_func = p; }
    void  setPtr(void * ptr) { m_ptr = ptr; }
    void  setMethod(std::string &method) { m_method = method; }

    unsigned dcsend(std::string s);
    int dcrecv(char *buf, int size);
    int dcconnect();

private:
    DCSocket();
    DCSocket(char* url);
    int createTcpSocket();

    char* getIp(char *host);
    std::string buildGetQuery(char *page);

    CALLBACK m_func;
    int     m_sock;
    char*   m_host;
    char*   m_url;
    int     m_port;
    void *  m_ptr;
    
    struct sockaddr_in *    m_remote;
    std::string             m_method;
    std::vector<char>       m_header;
    std::vector<char>       m_res;
};

} // namespace DCanvas

#endif //DCSOCKET_H
