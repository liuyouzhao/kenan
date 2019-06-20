#ifndef INT32ARRAY_H
#define INT32ARRAY_H

#include "ArrayBufferView.h"
#include "ArrayType.h"
#include "ObjectWrap.h"

namespace DCanvas
{

class Int32Array: public ArrayType<int>
{
public:
    static Int32Array* create(unsigned int length);
    static Int32Array* create(const int* array, unsigned int length);
    static Int32Array* create(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length);

    Int32Array* subarray(int start) const;
    Int32Array* subarray(int start, int end) const;

    virtual bool isIntArray() const { return true; }

    void set(unsigned int index, double value);
    static const char* getClassName()	{	return "Int32Array";	}
private:
    Int32Array(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length);
    friend class ArrayType<int>;
};

} // namespace DCanvas

#endif // INT32ARRAY_H
