#ifndef INT16ARRAY_H
#define INT16ARRAY_H

#include "ArrayBufferView.h"
#include "ArrayType.h"
#include "ObjectWrap.h"

namespace DCanvas
{

class Int16Array: public ArrayType<short>
{

public:
    static Int16Array* create(unsigned int length);
    static Int16Array* create(const short* array, unsigned int length);
    static Int16Array* create(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length);

    Int16Array* subarray(int start) const;
    Int16Array* subarray(int start, int end) const;

    virtual bool isShortArray() const { return true; }

    void set(unsigned int index, double value);
    static const char* getClassName()	{	return "Int16Array";	}
private:
    Int16Array(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length);
    friend class ArrayType<short>;
};

} // namespace DCanvas

#endif // INT16ARRAY_H
