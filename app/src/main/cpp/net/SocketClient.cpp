#include "defines.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "SocketClient.h"
#include "DirectCanvas.h"
#include "ThreadQueue.h"

#undef LOG_TAG
#define  LOG_TAG    "SocketClient"

namespace DCanvas
{

const int g_cSOCKET_CLOSED = CLOSED;
const int g_cSOCKET_OPENED = OPENED;
const int g_cSOCKET_ERROR = ERROR;

SocketClient::SocketClient(void* pcb):
    m_nPort(DEFAULT_PORT),
    m_nSockFd(0),
    m_nConRst(-1),
    m_nBufferedAmount(0),
    m_pIp((char*)"127.0.0.1"),
    m_bExit(false),
    m_pJsImpl(pcb)
{
    bzero(&m_pin, sizeof(m_pin));
    m_pin.sin_family = AF_INET;
    inet_pton(AF_INET, m_pIp, &m_pin.sin_addr);
    m_pin.sin_port = htons(m_nPort);
    m_nSockFd = socket(AF_INET, SOCK_STREAM, 0);
    m_pSendQueue = new ThreadQueue<SEND_PACKAGE>();
}

SocketClient::SocketClient(char* ip, int port, void* pcb):
    m_nPort(port),
    m_nSockFd(0),
    m_nConRst(-1),
    m_nBufferedAmount(0),
    m_pIp(ip),
    m_bExit(false),
    m_pJsImpl(pcb)
{
    bzero(&m_pin, sizeof(m_pin));
    m_pin.sin_family = AF_INET;
    inet_pton(AF_INET, m_pIp, &m_pin.sin_addr);
    m_pin.sin_port = htons(m_nPort);
    m_nSockFd = socket(AF_INET, SOCK_STREAM, 0);
    m_pSendQueue = new ThreadQueue<SEND_PACKAGE>();
}

SocketClient::~SocketClient()
{
    disconnect();
}

int SocketClient::receive(char (&buf)[MAXLEN], int len)
{
    int revlen = len < MAXLEN ? len : MAXLEN;
    int n = recv(m_nSockFd, m_buf, revlen, 0);
    memcpy(buf, m_buf, n);
    return n;
}

int SocketClient::receive(char* (*buf), int len)
{
    if (buf == NULL)
    {
        return -1;
    }
    int revlen = len < MAXLEN ? len : MAXLEN;
    int n = recv(m_nSockFd, m_buf, revlen, 0);
    memcpy(*buf, m_buf, revlen);
    return n;
}

int SocketClient::receive(char* (*buf))
{
    if (*buf != NULL)
    {
        delete[] *buf;
    }
    int n = recv(m_nSockFd, m_buf, MAXLEN, 0);
    *buf = new char[n];
    memcpy(*buf, m_buf, n);
    return n;
}

bool SocketClient::scConnect()
{
    int err = 0;
    err = pthread_create(&m_ctid, NULL, connect_thread_func, this);
    err |= pthread_create(&m_ntid, NULL, recv_thread_func, this);
    err |= pthread_create(&m_stid, NULL, send_thread_func, this);
    if (err != 0)
    {
        printf("can't create thread: %s\n", strerror(err));
    }
    return true;
}

void SocketClient::disconnect()
{
    m_bExit = true;
    if (-1 == m_nConRst)
    {
        return;
    }
    pthread_join(m_ctid, NULL);
    pthread_join(m_ntid, NULL);
    pthread_join(m_stid, NULL);

    close(m_nSockFd);
    m_nConRst = -1;

    SocketPackage* sp = new SocketPackage();
    sp->m_pWs = (WebSocketImpl*)getJsImpl();
    sp->m_nKind = CLOSED;
    sp->m_pData = NULL;

    if (!DirectCanvas::s_queue->Enter(sp))
    {
        delete sp;
        sp = NULL;
    }

    if (m_pSendQueue)
    {
        m_pSendQueue->DeleteAll();
        delete m_pSendQueue;
        m_pSendQueue = NULL;
    }
}

void SocketClient::scSend(char* data, int len)
{
    if (data == NULL || len == 0)
    {
        return;
    }
    SEND_PACKAGE* sp = new SEND_PACKAGE();
    memset(sp->data, 0, MAXLEN);
    int cplen = len < MAXLEN ? len : MAXLEN;
    sp->len = cplen;
    memcpy(sp->data, data, cplen);
    if (m_pSendQueue->Enter(sp))
    {
        bufferInc();
    }
}

int SocketClient::connectTcp()
{
    m_nConRst = connect(m_nSockFd, (sockaddr *)&m_pin, (socklen_t)sizeof(m_pin));
    if (-1 == m_nConRst)
    {
        perror("call connect");
    }
    return m_nConRst;
}

void* recv_thread_func(void* args)
{
    SocketClient* pself = (SocketClient*)args;
    pthread_join(pself->getCtid(), NULL);
    while (!pself->isExit())
    {
        char buf[MAXLEN] = {0};
        int n = pself->receive(buf, MAXLEN);
        if (n > 0)
        {
            SocketPackage* sp = new SocketPackage();
            sp->m_pWs = (WebSocketImpl*)pself->getJsImpl();
            sp->m_nKind = n;
            sp->m_pData = new char[n+1];
            memset(sp->m_pData, 0, n+1);
            memcpy(sp->m_pData, buf, n);
            if (!DirectCanvas::s_queue->Enter(sp))
            {
                delete sp;
                sp = NULL;
            }
        }
    }
    return NULL;
}

void* send_thread_func(void* args)
{
    SocketClient* pself = (SocketClient*)args;
    pthread_join(pself->getCtid(), NULL);
    while (!pself->isExit())
    {
        SEND_PACKAGE* sp = pself->getSendQueue()->Out();
        if (sp != NULL)
        {
             send(pself->getSocketFd(), sp->data, sp->len, 0);
             pself->bufferDev();
        }
        delete sp;
        usleep(SEND_BLOCK);
    }
    return NULL;
}

void* connect_thread_func(void* args)
{
    SocketClient* pself = (SocketClient*)args;
    int sts = pself->connectTcp();
    if (-1 == sts)
    {
        return NULL;
    }
    SocketPackage* sp = new SocketPackage();
    sp->m_pWs = (WebSocketImpl*)pself->getJsImpl();
    sp->m_nKind = 0;
    sp->m_pData = NULL;
    if (!DirectCanvas::s_queue->Enter(sp))
    {
        delete sp;
        sp = NULL;
    }
    return NULL;
}

SocketPackage::SocketPackage():
    m_pWs(NULL),
    m_nKind(NULL),
    m_pData(NULL)
{
}

SocketPackage::~SocketPackage()
{
    if (m_pData != NULL)
    {
        delete[] m_pData;
    }
}

void SocketPackage::handleMessage()
{
    if (m_pWs == NULL)
    {
        return;
    }
    WebSocketImpl* pCurrentWs = ((WebSocketImpl*)m_pWs);

    switch (m_nKind)
    {
        case g_cSOCKET_OPENED:
            pCurrentWs->onOpen();
            break;
        case g_cSOCKET_CLOSED:
            pCurrentWs->onClose();
            break;
        case g_cSOCKET_ERROR:
            pCurrentWs->onError();
            break;
        default:
            if (m_pWs > 0)
            {
                pCurrentWs->onMessage(m_pData, m_nKind);
            }
            break;
    }
}

} // namespace DCanvas
