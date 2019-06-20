#ifndef FLOAT32ARRAY_H
#define FLOAT32ARRAY_H

#include "ArrayBufferView.h"
#include "ArrayType.h"
#include "ObjectWrap.h"

namespace DCanvas
{

class Float32Array: public ArrayType<float>
{
public:
    static Float32Array* create(unsigned int length);
    static Float32Array* create(const float* array, unsigned int length);
    static Float32Array* create(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length);

    Float32Array* subarray(int start) const;
    Float32Array* subarray(int start, int end) const;

    virtual bool isFloatArray() const { return true; }
    virtual ~Float32Array();

    void set(unsigned int index, float value);
    static const char* getClassName()	{	return "Float32Array";	}

private:
    Float32Array(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length);
    friend class ArrayType<float>;
};

} // namespace DCanvas

#endif // FLOAT32ARRAY_H
