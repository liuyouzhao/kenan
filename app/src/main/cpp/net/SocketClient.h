#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <netinet/in.h>
#include <pthread.h>

#ifndef MAXLEN
#define MAXLEN 800
#endif
#ifndef ON_CONNECT
#define ON_CONNECT -520
#endif
#ifndef SEND_BLOCK
#define SEND_BLOCK 1800
#endif
#define DEFAULT_PORT 8000

namespace DCanvas
{

void* recv_thread_func(void* args);
void* send_thread_func(void* args);
void* connect_thread_func(void* args);
enum SOCKETSTATE
{
    CLOSED = -9,
    OPENED = 0,
    ERROR = -1
};

extern const int g_cSOCKET_CLOSED;
extern const int g_cSOCKET_OPENED;
extern const int g_cSOCKET_ERROR;

template<class Object>
class ThreadQueue;

typedef struct tagSEND_PACKAGE
{
    char    data[MAXLEN];
    int     len;
}SEND_PACKAGE;

class SocketPackage
{
public:
    SocketPackage();
    ~SocketPackage();
    void handleMessage();
    void*           m_pWs;
    int             m_nKind;
    char*           m_pData;
};

class SocketClient
{
public:
    SocketClient(void* pcb);
    SocketClient(char* ip, int port, void* pcb);
    virtual ~SocketClient();

    void        scSend(char* data, int len);
    bool        scConnect();
    void        disconnect();
    int         receive(char (&buf)[MAXLEN], int len);
    int         receive(char* (*buf), int len);
    int         receive(char* (*buf));

    bool        isExit()   { return m_bExit; }

    pthread_t   getNtid() { return m_ntid; }
    pthread_t   getStid() { return m_stid; }
    pthread_t   getCtid() { return m_ctid; }

    void*       getJsImpl() { return m_pJsImpl; }
    int         getSocketFd()    { return m_nSockFd; }

    ThreadQueue<SEND_PACKAGE>* getSendQueue() { return m_pSendQueue; }

    int         getBufferedAmount() { return m_nBufferedAmount; }
    void        getBufferedAmount(int i)    {    m_nBufferedAmount = i; }

    void        bufferInc() { ++m_nBufferedAmount; }
    void        bufferDev() { --m_nBufferedAmount; }
    int         connectTcp();

protected:
private:
    ThreadQueue<SEND_PACKAGE>*    m_pSendQueue;

    pthread_t      m_ntid;
    pthread_t      m_stid;
    pthread_t      m_ctid;

    struct sockaddr_in m_pin;

    int            m_nPort;
    int            m_nSockFd;
    int            m_nConRst;
    int            m_nBufferedAmount;

    char*          m_pIp;
    char           m_buf[MAXLEN];
    bool           m_bExit;
    void*          m_pJsImpl;
};

} // namespace DCanvas

#endif // SOCKETCLIENT_H
