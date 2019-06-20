#include "Uint8Array.h"
#include "defines.h"

#undef LOG_TAG
#define  LOG_TAG    "Uint8Array"

namespace DCanvas
{
Uint8Array* Uint8Array::create(unsigned int length)
{
    return ArrayType<unsigned char>::create<Uint8Array>(length);
}

Uint8Array* Uint8Array::create(const unsigned char* array, unsigned int length)
{
    return ArrayType<unsigned char>::create<Uint8Array>(array, length);
}

Uint8Array* Uint8Array::create(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length)
{
    return ArrayType<unsigned char>::create<Uint8Array>(buffer, byteOffset, length);
}

Uint8Array::Uint8Array(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length)
    : ArrayType<unsigned char>(buffer, byteOffset, length)
{

}

Uint8Array* Uint8Array::subarray(int start) const
{
    return subarray(start, length());
}

Uint8Array* Uint8Array::subarray(int start, int end) const
{
    Uint8Array* tmp = subarrayImpl<Uint8Array>(start, end);
    tmp->m_isSubArray = true;
    return tmp;
}

void Uint8Array::set(unsigned int index, double value)
{
    if (index > m_length)
    {
        LOGE("out of size error");
        return;
    }
    data()[index] = value;
}
} // namespace DCanvas
