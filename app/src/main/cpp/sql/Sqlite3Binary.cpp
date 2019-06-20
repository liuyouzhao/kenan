#include "Sqlite3Binary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
namespace DCanvas
{

int sqlite3_encode_binary(const unsigned char *in, int n, unsigned char *out);
int sqlite3_decode_binary(const unsigned char *in, unsigned char *out);

SQLite3Binary::SQLite3Binary()
        : m_pBuf(0)
        , m_nBinaryLen(0)
        , m_nBufferLen(0)
        , m_nEncodedLen(0)
        , m_bEncoded(false)
{
}


SQLite3Binary::~SQLite3Binary()
{
    clear();
}


void SQLite3Binary::setBinary(const unsigned char* pBuf, int nLen)
{
    m_pBuf = allocBuffer(nLen);
    memcpy(m_pBuf, pBuf, nLen);
}


void SQLite3Binary::setEncoded(const unsigned char* pBuf)
{
    clear();

    m_nEncodedLen = strlen((const char*)pBuf);
    m_nBufferLen = m_nEncodedLen + 1; // Allow for NULL terminator

    m_pBuf = (unsigned char*)malloc(m_nBufferLen);

    if (!m_pBuf)
    {
        //throw CSQLite3Exception(CPPSQLITE_ERROR, "Cannot allocate memory", DONT_DELETE_MSG);
    }

    memcpy(m_pBuf, pBuf, m_nBufferLen);
    m_bEncoded = true;
}


const unsigned char* SQLite3Binary::getEncoded()
{
    if (!m_bEncoded)
    {
        unsigned char* ptmp = (unsigned char*)malloc(m_nBinaryLen);
        memcpy(ptmp, m_pBuf, m_nBinaryLen);
        m_nEncodedLen = sqlite3_encode_binary(ptmp, m_nBinaryLen, m_pBuf);
        free(ptmp);
        m_bEncoded = true;
    }

    return m_pBuf;
}


const unsigned char* SQLite3Binary::getBinary()
{
    if (m_bEncoded)
    {
        // in/out buffers can be the same
        m_nBinaryLen = sqlite3_decode_binary(m_pBuf, m_pBuf);

        if (m_nBinaryLen == -1)
        {
            return NULL;//throw CSQLite3Exception(CPPSQLITE_ERROR, "Cannot decode binary", DONT_DELETE_MSG);
        }

        m_bEncoded = false;
    }

    return m_pBuf;
}


int SQLite3Binary::getBinaryLength()
{
    getBinary();
    return m_nBinaryLen;
}


unsigned char* SQLite3Binary::allocBuffer(int nLen)
{
    clear();

    // Allow extra space for encoded binary as per comments in
    // SQLite encode.c See bottom of this file for implementation
    // of SQLite functions use 3 instead of 2 just to be sure ;-)
    m_nBinaryLen = nLen;
    m_nBufferLen = 3 + (257*nLen)/254;

    m_pBuf = (unsigned char*)malloc(m_nBufferLen);

    if (!m_pBuf)
    {
        return NULL;//throw CSQLite3Exception(CPPSQLITE_ERROR, "Cannot allocate memory", DONT_DELETE_MSG);
    }

    m_bEncoded = false;

    return m_pBuf;
}


void SQLite3Binary::clear()
{
    if (m_pBuf)
    {
        m_nBinaryLen = 0;
        m_nBufferLen = 0;
        free(m_pBuf);
        m_pBuf = 0;
    }
}

int sqlite3_encode_binary(const unsigned char *in, int n, unsigned char *out)
{
    int i, j, e = 0, m;
    int cnt[256];
    if (n <= 0)
    {
        out[0] = 'x';
        out[1] = 0;
        return 1;
    }
    memset(cnt, 0, sizeof(cnt));
    for(i = n-1; i >= 0; i--)
    {
        cnt[in[i]]++;
    }
    m = n;
    for(i = 1; i < 256; i++)
    {
        int sum;
        if (i == '\'')
            continue;
        sum = cnt[i] + cnt[(i + 1) & 0xff] + cnt[(i + '\'') & 0xff];
        if(sum < m)
        {
          m = sum;
          e = i;
          if(m == 0)
              break;
        }
    }
    out[0] = e;
    j = 1;
    for(i = 0; i < n; i++)
    {
        int c = (in[i] - e) & 0xff;
        if(c == 0)
        {
          out[j++] = 1;
          out[j++] = 1;
        }
        else if(c == 1)
        {
          out[j++] = 1;
          out[j++] = 2;
        }
        else if(c == '\'')
        {
          out[j++] = 1;
          out[j++] = 3;
        }
        else
        {
          out[j++] = c;
        }
    }
    out[j] = 0;
    return j;
}

/*
** Decode the string "in" into binary data and write it into "out".
** This routine reverses the encoding created by sqlite3_encode_binary().
** The output will always be a few bytes less than the input.  The number
** of bytes of output is returned.  If the input is not a well-formed
** encoding, -1 is returned.
**
** The "in" and "out" parameters may point to the same buffer in order
** to decode a string in place.
*/
int sqlite3_decode_binary(const unsigned char *in, unsigned char *out)
{
    int i, c, e;
    e = *(in++);
    i = 0;
    while ((c = *(in++))!= 0)
    {
    if (c == 1 )
    {
        c = *(in++);
        if( c == 1 )
        {
            c = 0;
        }
        else if ( c == 2 )
        {
            c = 1;
        }
        else if( c == 3 )
        {
            c = '\'';
        }
        else
        {
            return -1;
        }
    }

    out[i++] = (c + e)&0xff;
    }
    return i;
}

} // namespace DCanvas
