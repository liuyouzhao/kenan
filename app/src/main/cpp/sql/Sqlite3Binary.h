#ifndef SQLITE3DBINARY_H
#define SQLITE3DBINARY_H

extern "C"
{
#include <sqlite3.h>
}

namespace DCanvas
{
class SQLite3Binary
{
public:

    SQLite3Binary();

    ~SQLite3Binary();

    void setBinary(const unsigned char* pBuf, int nLen);
    void setEncoded(const unsigned char* pBuf);

    const unsigned char* getEncoded();
    const unsigned char* getBinary();

    int getBinaryLength();

    unsigned char* allocBuffer(int nLen);

    void clear();

private:

    unsigned char* m_pBuf;
    int m_nBinaryLen;
    int m_nBufferLen;
    int m_nEncodedLen;
    bool m_bEncoded;
};

}// namespace DCanvas

#endif // SQLITE3DBINARY_H
