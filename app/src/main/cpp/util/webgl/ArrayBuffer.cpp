#include "defines.h"
#include "ArrayBuffer.h"


#undef LOG_TAG
#define  LOG_TAG    "ArrayBuffer"

namespace DCanvas
{
ArrayBuffer* ArrayBuffer::create(unsigned int numElements, unsigned int elementByteSize)
{
    return new ArrayBuffer(numElements, elementByteSize);
}

ArrayBuffer* ArrayBuffer::create(ArrayBuffer* other)
{
    return ArrayBuffer::create(other->data(), other->byteLength());
}

ArrayBuffer* ArrayBuffer::create(const void * data, unsigned int byteLength)
{
    char* newData = new char[byteLength];
    memcpy(newData, data, byteLength);
    return new ArrayBuffer(newData, byteLength);
}

ArrayBuffer::ArrayBuffer(void* data, unsigned int lenInByte)
    : m_data(data)
    , m_byteLength(lenInByte)
{

}

ArrayBuffer::ArrayBuffer(unsigned int numElements, unsigned int elementByteSize)
{
    m_byteLength = numElements * elementByteSize;

    if (m_byteLength <= 0)
    {
        return;
    }

    m_data = new char[m_byteLength];
}

ArrayBuffer::~ArrayBuffer()
{
    if (m_byteLength > 0 && m_data)
        delete[] (char*)m_data;
}
} // namespace DCanvas
