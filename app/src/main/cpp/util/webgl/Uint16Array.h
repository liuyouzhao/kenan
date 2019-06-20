#ifndef UINT16ARRAY_H
#define UINT16ARRAY_H

#include "ArrayType.h"
#include "ObjectWrap.h"
#include "ArrayBufferView.h"

namespace DCanvas
{

class Uint16Array: public ArrayType<unsigned short>
{

public:
    static Uint16Array* create(unsigned int length);
    static Uint16Array* create(const unsigned short* array, unsigned int length);
    static Uint16Array* create(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length);

    void set(unsigned int index, float value);

    Uint16Array* subarray(int start) const;
    Uint16Array* subarray(int start, int end) const;

    virtual bool isUnsignedShortArray() const { return true; }
    static const char* getClassName()	{	return "Uint16Array";	}
private:

    Uint16Array(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length);
    friend class ArrayType<unsigned short>;
};

} // namespace DCanvas

#endif // UINT16ARRAY_H
