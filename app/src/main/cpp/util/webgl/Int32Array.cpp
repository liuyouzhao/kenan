#include "Int32Array.h"
#include "defines.h"

#undef LOG_TAG
#define  LOG_TAG    "Int32Array"

namespace DCanvas
{
Int32Array* Int32Array::create(unsigned int length)
{
    return ArrayType<int>::create<Int32Array>(length);
}

Int32Array* Int32Array::create(const int* array, unsigned int length)
{
    return ArrayType<int>::create<Int32Array>(array, length);
}

Int32Array* Int32Array::create(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length)
{
    return ArrayType<int>::create<Int32Array>(buffer, byteOffset, length);
}

Int32Array::Int32Array(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length)
    : ArrayType<int>(buffer, byteOffset, length)
{

}

void Int32Array::set(unsigned int index, double value)
{
    if (index > m_length)
    {
        LOGE("out of size error");
        return;
    }
    data()[index] = value;
}

Int32Array* Int32Array::subarray(int start) const
{
    return subarray(start, length());
}

Int32Array* Int32Array::subarray(int start, int end) const
{
    Int32Array* tmp = subarrayImpl<Int32Array>(start, end);
    tmp->m_isSubArray = true;
    return tmp;
}
} // namespace DCanvas
