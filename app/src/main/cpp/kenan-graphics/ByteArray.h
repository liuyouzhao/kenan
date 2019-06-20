#ifndef BYTEARRAY_H
#define BYTEARRAY_H

#include <new>

namespace DCanvas
{

class ByteArray
{
public:
    unsigned length() const { return m_size; }

    void set(unsigned index, double value)
    {
        if (index >= m_size)
            return;
        if (!(value > 0)) // Clamp NaN to 0
            value = 0;
        else if (value > 255)
            value = 255;
        
        m_data[index] = static_cast<unsigned char>(value + 0.5);
    }

    void set(unsigned index, unsigned char value)
    {
        if (index >= m_size)
            return;
        
        m_data[index] = value;
    }

    bool get(unsigned index, unsigned char& result) const
    {
        if (index >= m_size)
            return false;
        
        result = m_data[index];
        return true;
    }

    unsigned char get(unsigned index) const
    {
        return m_data[index];
    }

    unsigned char* data() { return m_data; }
    static ByteArray* create(size_t size)
    {
        //unsigned char* buffer = new unsigned char[size + sizeof(ByteArray) - sizeof(size_t)];
        //return new (buffer) ByteArray(size);
        return new ByteArray(size);
    }
    ~ByteArray()
    {
        //delete[] reinterpret_cast<unsigned char*>(this);
        if (m_data != NULL)
        {
            delete[] m_data;
            m_data = NULL;
        }
    }
        
private:
    ByteArray(size_t size)
        : m_size(size)
    {
        m_data = new unsigned char[size];
    }
    size_t          m_size;
    unsigned char*  m_data;
};

} // namespace DCanvas

#endif // BYTEARRAY_H
