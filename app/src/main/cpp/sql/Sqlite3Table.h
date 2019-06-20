#ifndef SQLITE3TABLE_H
#define SQLITE3TABLE_H

extern "C"
{
#include <sqlite3.h>
}

#include <v8.h>
#include "ObjectWrap.h"

namespace DCanvas
{
class SQLite3Table : public ObjectWrap
{
public:

    SQLite3Table();

    SQLite3Table(const SQLite3Table& rTable);

    SQLite3Table(char** paszResults, int nRows, int nCols);

    virtual ~SQLite3Table();

    SQLite3Table& operator=(const SQLite3Table& rTable);

    int numFields();

    int numRows();

    const char* fieldName(int nCol);

    const char* fieldValue(int nField);
    const char* fieldValue(const char* szField);

    int getIntField(int nField );
    int getIntField(const char* szField);

    double getFloatField(int nField);
    double getFloatField(const char* szField);

    const char* getStringField(int nField);
    const char* getStringField(const char* szField);

    bool fieldIsNull(int nField);
    bool fieldIsNull(const char* szField);

    void setRow(int nRow);

    int finalize();

    v8::Handle<v8::Value> wrap();

    static v8::Handle<v8::Value> FieldName(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> NumFields(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> FieldValue(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> GetIntField(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> GetFloatField(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> GetStringField(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> FieldIsNull(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> NumRows(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> SetRow(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> Finalize(const v8::FunctionCallbackInfo<v8::Value>& args);
private:

    void checkResults();

    int     m_nCols;
    int     m_nRows;
    int     m_nCurrentRow;
    char**  m_paszResults;
    char*   m_errorMessage;
    bool    m_hasError;
};

} // namespace DCanvas

#endif // SQLITE3TABLE_H
