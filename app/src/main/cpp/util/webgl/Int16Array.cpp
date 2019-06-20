#include "Int16Array.h"
#include "defines.h"

#undef LOG_TAG
#define  LOG_TAG    "Int16Array"

namespace DCanvas
{
Int16Array* Int16Array::create(unsigned int length)
{
    return ArrayType<short>::create<Int16Array>(length);
}

Int16Array* Int16Array::create(const short* array, unsigned int length)
{
    return ArrayType<short>::create<Int16Array>(array, length);
}

Int16Array* Int16Array::create(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length)
{
    return ArrayType<short>::create<Int16Array>(buffer, byteOffset, length);
}

Int16Array::Int16Array(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length)
    : ArrayType<short>(buffer, byteOffset, length)
{

}

void Int16Array::set(unsigned int index, double value)
{
    if (index > m_length)
    {
        LOGE("out of size error");
        return;
    }
    data()[index] = value;
}

Int16Array* Int16Array::subarray(int start) const
{
    return subarray(start, length());
}

Int16Array* Int16Array::subarray(int start, int end) const
{
    Int16Array* tmp = subarrayImpl<Int16Array>(start, end);
    tmp->m_isSubArray = true;
    return tmp;
}
} // namespace DCanvas
