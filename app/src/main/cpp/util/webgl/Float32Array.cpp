#include "Float32Array.h"
#include "defines.h"

#undef LOG_TAG
#define  LOG_TAG    "Float32Array"

namespace DCanvas
{
Float32Array* Float32Array::create(unsigned int length)
{
    return ArrayType<float>::create<Float32Array>(length);
}

Float32Array* Float32Array::create(const float* array, unsigned int length)
{
    return ArrayType<float>::create<Float32Array>(array, length);
}

Float32Array* Float32Array::create(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length)
{
    return ArrayType<float>::create<Float32Array>(buffer, byteOffset, length);
}

Float32Array::Float32Array(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length)
    : ArrayType<float>(buffer, byteOffset, length)
{

}

Float32Array::~Float32Array()
{
}

void Float32Array::set(unsigned int index, float value)
{
    if (index > m_length)
    {
        LOGE("out of size error");
        return;
    }
    data()[index] = value;
}

Float32Array* Float32Array::subarray(int start) const
{
    return subarray(start, length());
}

Float32Array* Float32Array::subarray(int start, int end) const
{
    Float32Array* tmp = subarrayImpl<Float32Array>(start, end);
    tmp->m_isSubArray = true;
    return tmp;
}
}  // namespace DCanvas
