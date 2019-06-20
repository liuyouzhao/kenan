#include "defines.h"
#include "ArrayBufferView.h"
#include "Uint16Array.h"
#include "Uint32Array.h"
#include "Uint8Array.h"
#include "Int32Array.h"
#include "Int16Array.h"
#include "Int8Array.h"
#include "Float32Array.h"
#include "V8ArrayType.h"

#undef LOG_TAG
#define  LOG_TAG    "ArrayBufferView"

namespace DCanvas
{
ArrayBufferView::ArrayBufferView(ArrayBuffer* buffer, unsigned int byteOffset)
    : m_byteOffset(byteOffset)
    , m_buffer(buffer)
    , m_isSubArray(false)
{
    m_baseAddress = m_buffer ? (static_cast<char*>(m_buffer->data()) + m_byteOffset): NULL;
}

ArrayBufferView::~ArrayBufferView()
{
    if (m_buffer && !m_isSubArray)
    {
        LOGD("delete ArrayBufferView!");
        delete m_buffer;
        m_buffer = NULL;
    }
}

void ArrayBufferView::calculateOffsetAndLength(int start, int end, unsigned arraySize,
                                          unsigned* offset, unsigned* length)
{
    if (start < 0)
        start += arraySize;

    if (start < 0)
        start = 0;

    if (end < 0)
        end += arraySize;

    if (end < 0)
        end = 0;

    if (end < start)
        end = start;

    *offset = static_cast<unsigned>(start);
    *length = static_cast<unsigned>(end - start);
}
} // namespace DCanvas
