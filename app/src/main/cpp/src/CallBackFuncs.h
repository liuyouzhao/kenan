#ifndef CALLBACKFUNCS_H
#define CALLBACKFUNCS_H

#include <list>
#include <pthread.h>

namespace DCanvas
{

typedef void* (*FUNCCALL)(void*);
struct FuncData
{
    FUNCCALL data;
    void * param;
};

class CallBackFuncs
{
public:
    static CallBackFuncs* getFuncQueue();
    ~CallBackFuncs();
    void   AddFunc(FuncData* p);
    void   CallAllFunc();
    static void   destroy();

private:
    static CallBackFuncs*   s_self;
    CallBackFuncs();
    std::list<FuncData*>    m_funcQueue;
    pthread_mutex_t         m_mutex;
};

} // namespace DCanvas

#endif // CALLBACKFUNCS_H
