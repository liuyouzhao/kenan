#ifndef ARRAYTYPE_H
#define ARRAYTYPE_H

/***
*this class is only to use template
*
***/
#include "ArrayBufferView.h"

namespace DCanvas
{

template <typename T>
class ArrayType: public ArrayBufferView
{
public:
    T* data() { return static_cast<T*>(baseAddress()); }
    template<class SubClass>
    static SubClass* create(unsigned int len)
    {
        ArrayBuffer* buffer = ArrayBuffer::create(len, sizeof(T));
        return create<SubClass>(buffer, 0, len);
    }

    template<class SubClass>
    static SubClass* create(const T* data, unsigned int len)
    {
        SubClass* a = create<SubClass>(len);
        if(a)
        {
            for(unsigned int i=0;i<len;++i)
            {
                a->set(i,data[i]);
            }
        }
        return a;
    }
    template<class SubClass>
    static SubClass* create(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int len)
    {
        return new SubClass(buffer, byteOffset, len);
    }

    template <class Subclass>
    Subclass* subarrayImpl(int start, int end) const
    {
        unsigned offset, length;
        calculateOffsetAndLength(start, end, m_length, &offset, &length);

        clampOffsetAndNumElements<T>(buffer(), m_byteOffset, &offset, &length);

        return create<Subclass>(buffer(), offset, length);
    }

    unsigned int length() const {return m_length;}
    unsigned int byteLength() const {return m_length * sizeof(T);}

protected:
    ArrayType(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length)
    : ArrayBufferView(buffer, byteOffset)
    , m_length(length)
    {
    }

    unsigned int m_length;
};

} // namespace DCanvas

#endif // ARRAYTYPE_H
