#include "defines.h"
#include "CallBackFuncs.h"

#undef LOG_TAG
#define  LOG_TAG    "CallBackFuncs"

namespace DCanvas
{

CallBackFuncs* CallBackFuncs::s_self = NULL;
CallBackFuncs* CallBackFuncs::getFuncQueue()
{
    if(!s_self)
        s_self = new CallBackFuncs();

    return s_self;
}

CallBackFuncs::CallBackFuncs()
{
    m_funcQueue.clear();
    pthread_mutex_init(&m_mutex, NULL);
}

CallBackFuncs::~CallBackFuncs()
{
    if (m_funcQueue.size() > 0)
    {
       pthread_mutex_lock(&m_mutex);

        for(std::list<FuncData*>::iterator it = m_funcQueue.begin(); it != m_funcQueue.end(); ++it)
        {
            FuncData* fd = *it;
            delete fd;
        }

        m_funcQueue.clear();
        pthread_mutex_unlock(&m_mutex);
    }
}

void CallBackFuncs::destroy()
{
    if (s_self)
        delete s_self;
    s_self = NULL;
}

void CallBackFuncs::AddFunc(FuncData* p)
{
    pthread_mutex_lock(&m_mutex);
    m_funcQueue.push_back(p);
    pthread_mutex_unlock(&m_mutex);
}

void CallBackFuncs::CallAllFunc()
{
   pthread_mutex_lock(&m_mutex);

    for(std::list<FuncData*>::iterator it = m_funcQueue.begin(); it != m_funcQueue.end(); ++it)
    {
        FuncData* fd = *it;
        ((FUNCCALL)fd->data)(fd->param);
        delete fd;
    }

    m_funcQueue.clear();
    pthread_mutex_unlock(&m_mutex);
}

}// namespace DCanvas
