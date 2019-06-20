#include "defines.h"
#include "DCSocket.h"

#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cctype>

#undef LOG_TAG
#define  LOG_TAG    "DCSocket"

namespace DCanvas
{

DCSocket* DCSocket::create()
{
    return new DCSocket();
}

DCSocket* DCSocket::create(char* url)
{
    return new DCSocket(url);
}

DCSocket::DCSocket()
{
    m_url = NULL;
    m_host = NULL;
    m_port = 80;
    m_sock = -1;
    m_method = "GET";
    createTcpSocket();
    m_res.clear();
    m_header.clear();
    m_func = NULL;
}

DCSocket::DCSocket(char* url)
{
    m_url = NULL;
    m_host = NULL;
    m_port = 80;
    m_sock = -1;
    m_method = "GET";
    setUrl(url);
    createTcpSocket();
    m_res.clear();
    m_header.clear();
    m_func = NULL;
}

DCSocket::~DCSocket()
{
    if (m_url)
        delete[] m_url;
    if (m_host)
        delete[] m_host;
    if (0 < m_sock)
        close(m_sock);
}

void DCSocket::setUrl(char* url)
{
    m_url = new char[strlen(url) + 1];
    bzero(m_url, strlen(url) + 1);
    memcpy(m_url, url, strlen(url));
    m_host = getIp(url);
}

int DCSocket::createTcpSocket()
{
    if ((m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        LOGE("Can't create TCP socket");
        return -1;
    }
    return m_sock;
}

char* DCSocket::getIp(char *host)
{
    struct hostent *hent;

    struct addrinfo *answer, hint, *curr;
    char* ipstr = new char[33];
    bzero(&hint, sizeof(hint));
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;

    int ret = getaddrinfo(m_url, NULL, &hint, &answer);
    if (ret != 0)
    {
        LOGE("url is %s", m_url);
        LOGE("getaddressinfo wrong! info %s", gai_strerror(ret));
        return NULL;
    }

    for (curr = answer; curr != NULL; curr = curr->ai_next)
    {
        if (inet_ntop(AF_INET, &(((struct sockaddr_in *)(curr->ai_addr))->sin_addr), ipstr, 16) == NULL)
        {
            LOGE("Can't resolve host");
            continue;
        }
        printf("%s\n", ipstr);
        return ipstr;
    }
    return NULL;
}

char* DCSocket::recieve(char* path)
{
    if (m_host == NULL)
    {
        LOGE("net is not ok");
        return NULL;
    }
    int i = 0;
    struct sockaddr_in *remote;
    char buf[BUFFSIZE + 1];

    remote = new struct sockaddr_in;
    remote->sin_family = AF_INET;
    int tmpres = inet_pton(AF_INET, m_host, (void *)(&(remote->sin_addr.s_addr)));

    if (tmpres < 0)
    {
        LOGE("Can't set remote->sin_addr.s_addr");
        return NULL;
    }
    else if (tmpres == 0)
    {
        LOGE("%s is not a valid IP address\n", m_host);
        return NULL;
    }

    remote->sin_port = htons(m_port);
    if (connect(m_sock, (struct sockaddr *)remote, sizeof(struct sockaddr)) < 0)
    {
        LOGE("Could not connect");
        return NULL;
    }

    std::string get = buildGetQuery(path);

    //Send the query to the server
    unsigned sent = 0;
    unsigned int glen = get.length();
    while (sent < glen)
    {
        tmpres = send(m_sock, get.c_str()+sent, glen-sent, 0);
        //LOGD("Send result tag: %d ", tmpres);
        if (tmpres == -1)
        {
            LOGE("Can't send query");
            return NULL;
        }
        sent += tmpres;
    }

    //now it is time to receive the page
    memset(buf, 0, sizeof(buf));
    int htmlstart = 0;
    char * htmlcontent;
    while ((tmpres = read(m_sock, buf, BUFFSIZE)) > 0)
    {
        if (htmlstart == 0)
        {
            /* Under certain conditions this will not work.
            * If the \r\n\r\n part is splitted into two messages
            * it will fail to detect the beginning of HTML content
            */
            htmlcontent = strstr(buf, "\r\n\r\n");

            if (htmlcontent != NULL)
            {
                for (i=0; buf+i != htmlcontent; ++i)
                {
                    m_header.push_back(buf[i]);
                }
                htmlstart = 1;
                htmlcontent += 4;
                for (char* s = htmlcontent; s != buf + tmpres; ++s)
                {
                    m_res.push_back(*s);
                }
            }
            else
            {
                for (i=0; i < tmpres; ++i)
                    m_header.push_back(buf[i]);
            }
        }
        else
        {
            htmlcontent = buf;
            for (i = 0; i<tmpres; ++i)
                m_res.push_back(htmlcontent[i]);
        }
        memset(buf, 0, tmpres);
    }
    if (tmpres < 0)
    {
        LOGE("Error receiving data");
    }

    if (remote)
    {
        delete remote;
    }

    if (m_func)
    {
        m_func((void*)&m_res[0], m_res.size(), m_ptr);
    }
    return &m_res[0];
}

#define USERAGENT "HTMLGET 1.0"
std::string DCSocket::buildGetQuery(char *page)
{
    std::string query("");
    char *getpage = page;
    if (getpage[0] == '/')
    {
        getpage = getpage + 1;
        fprintf(stderr,"Removing leading \"/\", converting %s to %s\n", page, getpage);
    }
    query += m_method+" /" + std::string(getpage) + " HTTP/1.0\r\nHost: "+
             std::string(m_url) +"\r\nUser-Agent: HTMLGET 1.0\r\n\r\n";

    return query;
}

unsigned DCSocket::dcsend(std::string s)
{
    int sent = 0;
    int tmpres = 0;
    int len = s.length();
    char* get = (char*)s.c_str();

    while (sent < len)
    {
        tmpres = send(m_sock, get+sent, len-sent, 0);
        if (tmpres == -1)
        {
            LOGE("Can't send query");
            return false;
        }

        sent += tmpres;
    }
    return true;
}

int DCSocket::dcrecv(char *buf, int size)
{
    return recv(m_sock, buf, size, 0);
}

int DCSocket::dcconnect()
{
    struct sockaddr_in *remote;
    if (m_host == NULL)
    {
        LOGE("net is not ok");
        return -1;
    }

    remote = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));
    remote->sin_family = AF_INET;
    int tmpres = inet_pton(AF_INET, m_host, (void *)(&(remote->sin_addr.s_addr)));

    if (tmpres < 0)
    {
        LOGE("Can't set remote->sin_addr.s_addr");
        return -1;
    }
    else if (tmpres == 0)
    {
        LOGE("%s is not a valid IP address\n", m_host);
        return 0;
    }

    remote->sin_port = htons(m_port);
    if (connect(m_sock, (struct sockaddr *)remote, sizeof(struct sockaddr)) < 0)
    {
        LOGE("Could not connect");
        return -1;
    }

    return tmpres;
}

bool DCSocket::parse(const char* url, int len, std::string& method, std::string& host,
                     int &port, std::string& path, std::string& param)
{
    //http://www.test.com[:80][/path][?...]
    method.clear();
    host.clear();
    path.clear();
    param.clear();
    int i=0, j=0;

    //http://
    while (i < len)/*parse protocol*/
    {
        if (url[i]==':')
        {
            break;
        }
        else
        {
            method += url[i] > 96 ? url[i] - 32 : url[i];
            ++i;
        }
    }

    if (i == 0 || i == len)
        return false;

    ++i;
    while (i < len)/*delete /*/
    {
        if (url[i]=='/')
            ++i;
        else
            break;
    }

    while (i < len)
    {

        if (std::isalpha(url[i]) || url[i] == '.'
         || std::isdigit(url[i]) || url[i]=='-')/*host*/
        {
            if (host.length() > 0)
                return false;

            while (i < len)
            {
                if (std::isalpha(url[i]) || url[i] == '.'
                 || std::isdigit(url[i]) || url[i]=='-')
                {
                    host += url[i];
                    ++i;
                }else
                    break;
            }

        }
        else if (url[i] == '/')/*path*/
        {
            if (path.length() > 0)
                return false;
            path="/";
            ++i;
            while (i < len)
            {
                if (url[i] == '?')
                    break;
                else
                    path += url[i++];
            }
        }
        else if (url[i] == ':')/*port*/
        {
            if (port != 80)
                return false;
            ++i;
            port = 0;
            while (i < len)
            {
                if (!std::isdigit(url[i]))
                {
                    break;
                }
                else
                {
                    port = port * 10 + url[i++] - '0';
                }
            }
            if (port == 0)
                port = 80;
        }
        else if (url[i] == '?')/*param*/
        {
            if (param.length() > 0)
                return false;
            ++i;
            while (i < len)
            {
                param += url[i++];
            }
        }
        else
        {
            LOGE("URL error");
            return false;
        }
    }
    return true;
}

}  //namespace DCanvas
