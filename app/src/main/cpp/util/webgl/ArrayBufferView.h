#ifndef ARRAYBUFFERVIEW_H
#define ARRAYBUFFERVIEW_H

#include "ArrayBuffer.h"
#include "ObjectWrap.h"

namespace DCanvas
{

class ArrayBufferView
{
public:
    virtual ~ArrayBufferView();
    virtual bool isByteArray() const { return false; }
    virtual bool isUnsignedByteArray() const { return false; }
    virtual bool isShortArray() const { return false; }
    virtual bool isUnsignedShortArray() const { return false; }
    virtual bool isIntArray() const { return false; }
    virtual bool isUnsignedIntArray() const { return false; }
    virtual bool isFloatArray() const { return false; }
    virtual bool isDoubleArray() const { return false; }
    virtual bool DataView() const { return false; }
    ArrayBuffer* buffer() const { return m_buffer; }
    void* baseAddress() { return m_baseAddress; }
    unsigned int byteOffset() const { return m_byteOffset; }
    virtual unsigned int byteLength() const = 0;
    static void calculateOffsetAndLength(int start, int end, unsigned arraySize,
							unsigned* offset, unsigned* length);
    template <typename T>
    static void clampOffsetAndNumElements(ArrayBuffer* buffer,
										 unsigned arrayByteOffset,
										 unsigned *offset,
										 unsigned *numElements)
    {
	    unsigned maxOffset = (0x7FFFFFFF - arrayByteOffset) / sizeof(T);
	    if (*offset > maxOffset) {
 		   *offset = buffer->byteLength();
	 	   *numElements = 0;
	 	   return;
	    }
	    *offset = arrayByteOffset + *offset * sizeof(T);
	    *offset = buffer->byteLength() > *offset ? *offset : buffer->byteLength();
	    unsigned remainingElements = (buffer->byteLength() - *offset) / sizeof(T);
	    *numElements = remainingElements > *numElements ? *numElements : remainingElements ;
    }
protected:
    ArrayBufferView(ArrayBuffer*, unsigned int);
    unsigned int m_byteOffset;

private:
    void *          m_baseAddress;
    ArrayBuffer *   m_buffer;

protected:
    bool     m_isSubArray;
};

} //namespace DCanvas

#endif //ARRAYBUFFERVIEW_H
