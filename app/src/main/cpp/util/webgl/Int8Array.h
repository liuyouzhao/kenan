#ifndef INT8ARRAY_H
#define INT8ARRAY_H

#include "ArrayBufferView.h"
#include "ArrayType.h"
#include "ObjectWrap.h"

namespace DCanvas
{

class Int8Array: public ArrayType<signed char>
{

public:
    static Int8Array* create(unsigned int length);
    static Int8Array* create(const signed char* array, unsigned int length);
    static Int8Array* create(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length);

    Int8Array* subarray(int start) const;
    Int8Array* subarray(int start, int end) const;

    virtual bool isByteArray() const { return true; }

    void set(unsigned int index, double value);
    static const char* getClassName()	{	return "Int8Array";	}
private:
    Int8Array(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length);
    friend class ArrayType<signed char>;
};

} // namespace DCanvas

#endif //INT8ARRAY_H
