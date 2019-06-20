#ifndef THREADQUEUE_H
#define THREADQUEUE_H

#include <pthread.h>
#include <stdio.h>

namespace DCanvas
{
#ifndef MAXLEN
#define MAXLEN 800
#endif

const int QUEUESIZE = 20;

template<class Object>
class ThreadQueue
{
public:
    ThreadQueue();
    ~ThreadQueue();
public:
    bool Enter(Object *obj);

    Object* Out();
    Object* GetAt(int id);

    bool IsEmpty();
    bool IsFull();
    void DeleteAll();
private:
    int m_nFront;
    int m_nRear;
    int m_nSize;

    Object * m_pList[QUEUESIZE];
    pthread_mutex_t m_queueMutex;
};

template<class Object>
ThreadQueue<Object>::ThreadQueue()
{
    m_nFront = m_nRear = 0;
    m_nSize = QUEUESIZE;
    for (int i = 0; i < m_nSize; i ++)
    {
        m_pList[i] = NULL;
    }
    pthread_mutex_init(&m_queueMutex, NULL);
}

template<class Object>
bool ThreadQueue<Object>::Enter(Object* obj)
{
    pthread_mutex_lock(&m_queueMutex);
    if (IsFull())
    {
        pthread_mutex_unlock(&m_queueMutex);
        return false;
    }
    m_pList[m_nRear] = obj;
    m_nRear = (m_nRear + 1) % m_nSize;
    pthread_mutex_unlock(&m_queueMutex);
    return true;
}

template<class Object>
Object* ThreadQueue<Object>::Out()
{
    Object* temp;
    pthread_mutex_lock(&m_queueMutex);
    if (IsEmpty())
    {
        pthread_mutex_unlock(&m_queueMutex);
        return false;
    }
    temp = m_pList[m_nFront];
    m_pList[m_nFront] = NULL;
    m_nFront = (m_nFront + 1) % m_nSize;
    pthread_mutex_unlock(&m_queueMutex);
    return temp;
}

template<class Object>
Object* ThreadQueue<Object>::GetAt(int id)
{
    Object* temp;
    pthread_mutex_lock(&m_queueMutex);
    if (IsEmpty() || id >= m_nSize)
    {
        pthread_mutex_unlock(&m_queueMutex);
        return false;
    }
    temp = m_pList[id];
    pthread_mutex_unlock(&m_queueMutex);

    return temp;
}

template<class Object>
bool ThreadQueue<Object>::IsEmpty()
{
    if (m_nRear == m_nFront)
        return true;
    else
        return false;
}

template<class Object>
bool ThreadQueue<Object>::IsFull()
{
    if ((m_nRear + 1) % m_nSize == m_nFront)
        return true;
    else
        return false;
}

template<class Object>
void ThreadQueue<Object>::DeleteAll()
{
    pthread_mutex_lock(&m_queueMutex);
    for (int i = 0; i < m_nSize; i ++)
    {
        if (m_pList[i] != NULL)
        {
            delete m_pList[i];
        }
    }
    pthread_mutex_unlock(&m_queueMutex);
}


template<class Object>
ThreadQueue<Object>::~ThreadQueue()
{
    //delete it self
    /*
    pthread_mutex_lock(&m_queueMutex);
    for (int i = 0; i < m_nSize; i ++)
    {
        if (m_pList[i] != NULL)
        {
            delete m_pList[i];
        }
    }
    pthread_mutex_unlock(&m_queueMutex);
    */
}

} // namespace DCanvas

#endif //THREADQUEUE_H
