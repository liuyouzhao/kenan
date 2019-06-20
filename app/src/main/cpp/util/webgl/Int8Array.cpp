#include "Int8Array.h"
#include "defines.h"

#undef LOG_TAG
#define  LOG_TAG    "Int8Array"

namespace DCanvas
{
Int8Array* Int8Array::create(unsigned int length)
{
    return ArrayType<signed char>::create<Int8Array>(length);
}

Int8Array* Int8Array::create(const signed char* array, unsigned int length)
{
    return ArrayType<signed char>::create<Int8Array>(array, length);
}

Int8Array* Int8Array::create(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length)
{
    return ArrayType<signed char>::create<Int8Array>(buffer, byteOffset, length);
}

Int8Array::Int8Array(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length)
    : ArrayType<signed char>(buffer, byteOffset, length)
{

}

void Int8Array::set(unsigned int index, double value)
{
    if (index > m_length)
    {
        LOGE("out of size error");
        return;
    }
    data()[index] = value;
}

Int8Array* Int8Array::subarray(int start) const
{
    return subarray(start, length());
}

Int8Array* Int8Array::subarray(int start, int end) const
{
    Int8Array* tmp = subarrayImpl<Int8Array>(start, end);
    tmp->m_isSubArray = true;
    return tmp;
}
} // namespace DCanvas
