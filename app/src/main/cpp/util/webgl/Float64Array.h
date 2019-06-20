#ifndef FLOAT64ARRAY_H
#define FLOAT64ARRAY_H

#include "ArrayBufferView.h"
#include "ArrayType.h"
#include "ObjectWrap.h"

namespace DCanvas
{

class Float64Array: public ArrayType<double>
{
public:
    static Float64Array* create(unsigned int length);
    static Float64Array* create(const double* array, unsigned int length);
    static Float64Array* create(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length);

    Float64Array* subarray(int start) const;
    Float64Array* subarray(int start, int end) const;

    virtual bool isDoubleArray() const { return true; }

    void set(unsigned int index, double value);
    static const char* getClassName()	{	return "Float64Array";	}
private:
    Float64Array(ArrayBuffer* buffer, unsigned int byteOffset, unsigned int length);
    friend class ArrayType<double>;
};

} // namespace DCanvas

#endif // FLOAT64ARRAY_H
