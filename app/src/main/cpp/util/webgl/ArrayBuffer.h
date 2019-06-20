#ifndef ARRAYBUFFER_H
#define ARRAYBUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include "ObjectWrap.h"

namespace DCanvas
{

class ArrayBuffer
{
public:
    static ArrayBuffer* create(unsigned int numElements, unsigned int elementByteSize);
    static ArrayBuffer* create(ArrayBuffer*);
    static ArrayBuffer* create(const void * data, unsigned int byteLength);

    virtual ~ArrayBuffer();
    void* data() { return m_data;}
    unsigned int byteLength() const { return m_byteLength; }
private:
    ArrayBuffer(void* data, unsigned int lenInByte);
    ArrayBuffer(unsigned int numElements, unsigned int elementByteSize);

    void *          m_data;
    unsigned int    m_byteLength;
};

} // namespace DCanvas

#endif // ARRAYBUFFER_H
