#include "Float64Array.h"
#include "defines.h"

#undef LOG_TAG
#define  LOG_TAG    "Float64Array"

namespace DCanvas
{
Float64Array* Float64Array::create(unsigned int length)
{
    return ArrayType<double>::create<Float64Array>(length);
}

Float64Array* Float64Array::create(const double* array, unsigned int length)
{
    return ArrayType<double>::create<Float64Array>(array, length);
}

Float64Array* Float64Array::create(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length)
{
    return ArrayType<double>::create<Float64Array>(buffer, byteOffset, length);
}

Float64Array::Float64Array(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length)
    : ArrayType<double>(buffer, byteOffset, length)
{

}

void Float64Array::set(unsigned int index, double value)
{
    if (index > m_length)
    {
        LOGE("out of size error");
        return;
    }
    data()[index] = value;
}

Float64Array* Float64Array::subarray(int start) const
{
    return subarray(start, length());
}

Float64Array* Float64Array::subarray(int start, int end) const
{
    Float64Array* tmp = subarrayImpl<Float64Array>(start, end);
    tmp->m_isSubArray = true;
    return tmp;
}
} // namespace DCanvas
