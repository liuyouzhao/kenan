#ifndef UINT8ARRAY_H
#define UINT8ARRAY_H

#include "ArrayBufferView.h"
#include "ArrayType.h"
#include "ObjectWrap.h"

namespace DCanvas
{

class Uint8Array: public ArrayType<unsigned char>
{
public:
    static Uint8Array* create(unsigned int length);
    static Uint8Array* create(const unsigned char* array, unsigned int length);
    static Uint8Array* create(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length);

    Uint8Array* subarray(int start) const;
    Uint8Array* subarray(int start, int end) const;

    virtual bool isUnsignedByteArray() const { return true; }

    void set(unsigned int index, double value);
    static const char* getClassName()	{	return "Uint8Array";	}
private:
    Uint8Array(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length);
    friend class ArrayType<unsigned char>;
};

} // namespace DCanvas

#endif // UINT8ARRAY_H
