#ifndef UINT32ARRAY_H
#define UINT32ARRAY_H

#include "ArrayBufferView.h"
#include "ArrayType.h"
#include "ObjectWrap.h"

namespace DCanvas
{

class Uint32Array: public ArrayType<unsigned int>
{
public:
    static Uint32Array* create(unsigned int length);
    static Uint32Array* create(const unsigned int* array, unsigned int length);
    static Uint32Array* create(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length);

    Uint32Array* subarray(int start) const;
    Uint32Array* subarray(int start, int end) const;

    virtual bool isUnsignedIntArray() const { return true; }

    void set(unsigned int index, double value);
    static const char* getClassName()	{	return "Uint32Array";	}
private:
    Uint32Array(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length);
    friend class ArrayType<unsigned int>;
};

} // namespace DCanvas

#endif // UINT32ARRAY_H
