#include "Uint32Array.h"
#include "defines.h"

#undef LOG_TAG
#define  LOG_TAG    "Uint32Array"

namespace DCanvas
{
Uint32Array* Uint32Array::create(unsigned int length)
{
    return ArrayType<unsigned int>::create<Uint32Array>(length);
}

Uint32Array* Uint32Array::create(const unsigned int* array, unsigned int length)
{
    return ArrayType<unsigned int>::create<Uint32Array>(array, length);
}

Uint32Array* Uint32Array::create(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length)
{
    return ArrayType<unsigned int>::create<Uint32Array>(buffer, byteOffset, length);
}

Uint32Array::Uint32Array(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length)
    : ArrayType<unsigned int>(buffer, byteOffset, length)
{

}

void Uint32Array::set(unsigned int index, double value)
{
    if (index > m_length)
    {
        LOGE("out of size error");
        return;
    }
    data()[index] = value;
}

Uint32Array* Uint32Array::subarray(int start) const
{
    return subarray(start, length());
}

Uint32Array* Uint32Array::subarray(int start, int end) const
{
    Uint32Array* tmp = subarrayImpl<Uint32Array>(start, end);
    tmp->m_isSubArray = true;
    return tmp;
}
} // namespace DCanvas
