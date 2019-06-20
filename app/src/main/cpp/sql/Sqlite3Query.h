#ifndef SQLITE3QUERY_H
#define SQLITE3QUERY_H

extern "C"
{
#include <sqlite3.h>
}

#include <v8.h>
#include "ObjectWrap.h"

namespace DCanvas
{
class SQLite3Query: public ObjectWrap
{
public:

    SQLite3Query();

    SQLite3Query(const SQLite3Query& rQuery);

    SQLite3Query(sqlite3* pDB,
                sqlite3_stmt* pVM,
                bool bEof,
                bool bOwnVM=true);

    SQLite3Query& operator=(const SQLite3Query& rQuery);

    virtual ~SQLite3Query();

    int numFields();

    int fieldIndex(const char* szField);
    const char* fieldName(int nCol);

    const char* fieldDeclType(int nCol);
    int fieldDataType(int nCol);

    const char* fieldValue(int nField);
    const char* fieldValue(const char* szField);

    // return -1 if no col
    int getIntField(int nField);
    int getIntField(const char* szField);

    // return -1.0 if no col
    double getFloatField(int nField);
    double getFloatField(const char* szField);

    // return NULL if no col
    const char* getStringField(int nField);
    const char* getStringField(const char* szField);

    const unsigned char* getBlobField(int nField, int& nLen);
    const unsigned char* getBlobField(const char* szField, int& nLen);

    bool fieldIsNull(int nField);
    bool fieldIsNull(const char* szField);

    bool eof();

    void nextRow();

    int finalize();

    v8::Handle<v8::Value> wrap();

    static v8::Handle<v8::Value> NumFields(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> FieldName(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> FieldDeclType(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> FieldIndex(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> FieldValue(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> GetIntField(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> GetFloatField(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> GetBlobField(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> FieldIsNull(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> Eof(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> NextRow(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> Finalize(const v8::FunctionCallbackInfo<v8::Value>& args);

private:

    void checkVM();

    sqlite3*        m_pDB;
    sqlite3_stmt*   m_pVM;
    bool            m_bEof;
    int             m_nCols;
    bool            m_bOwnVM;
    char *          m_errorMessage;
    bool            m_hasError;
};
}// namespace DCanvas
#endif //SQLITE3QUERY_H
