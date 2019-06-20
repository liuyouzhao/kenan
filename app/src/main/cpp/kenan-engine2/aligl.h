#ifndef  ALIGL_H
#define  ALIGL_H
#if NATIVE_SURFACE

#include "NativeEnv.h"

class aligl
{
public:
    aligl(){ m_nativeEnv = NULL; }

protected:
    NativeEnv* m_nativeEnv;
};

#endif //NATIVE_SURFAC

#endif // ALIGL_H
