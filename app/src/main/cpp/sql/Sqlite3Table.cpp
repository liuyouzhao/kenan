#include "Sqlite3Table.h"
#include <stdlib.h>
#include <string.h>
#include "defines.h"

#undef LOG_TAG
#define  LOG_TAG    "SQLite3Table"

namespace DCanvas
{

SQLite3Table::SQLite3Table()
{
    m_paszResults = 0;
    m_nRows = 0;
    m_nCols = 0;
    m_nCurrentRow = 0;
    m_hasError = false;
}


SQLite3Table::SQLite3Table(const SQLite3Table& rTable)
{
    m_paszResults = rTable.m_paszResults;
    // Only one object can own the results
    const_cast<SQLite3Table&>(rTable).m_paszResults = 0;
    m_nRows = rTable.m_nRows;
    m_nCols = rTable.m_nCols;
    m_nCurrentRow = rTable.m_nCurrentRow;
    m_hasError = false;
}


SQLite3Table::SQLite3Table(char** paszResults, int nRows, int nCols)
{
    m_paszResults = paszResults;
    m_nRows = nRows;
    m_nCols = nCols;
    m_nCurrentRow = 0;
    m_hasError = false;
}

SQLite3Table::~SQLite3Table()
{
    if(-1 == finalize()){}
}

SQLite3Table& SQLite3Table::operator=(const SQLite3Table& rTable)
{
    if(-1 == finalize()){}
    m_paszResults = rTable.m_paszResults;
    // Only one object can own the results
    const_cast<SQLite3Table&>(rTable).m_paszResults = 0;
    m_nRows = rTable.m_nRows;
    m_nCols = rTable.m_nCols;
    m_nCurrentRow = rTable.m_nCurrentRow;
    return *this;
}


int SQLite3Table::finalize()
{
    if (m_paszResults)
    {
        sqlite3_free_table(m_paszResults);
        m_paszResults = 0;
    }
    return 0;
}


int SQLite3Table::numFields()
{
    checkResults();
    return m_nCols;
}


int SQLite3Table::numRows()
{
    checkResults();
    return m_nRows;
}


const char* SQLite3Table::fieldValue(int nField)
{
    checkResults();

    if (nField < 0 || nField > m_nCols-1)
    {
        LOGE("field value request error");
        m_hasError = true;
        m_errorMessage = (char *)"Invalid field index requested";
        return NULL;
    }
    int nIndex = (m_nCurrentRow*m_nCols) + m_nCols + nField;
    return m_paszResults[nIndex];
}


const char* SQLite3Table::fieldValue(const char* szField)
{
    checkResults();

    if (szField)
    {
        for (int nField = 0; nField < m_nCols; nField++)
        {
            if (strcmp(szField, m_paszResults[nField]) == 0)
            {
                int nIndex = (m_nCurrentRow*m_nCols) + m_nCols + nField;
                return m_paszResults[nIndex];
            }
        }
    }
    LOGE("field value request error const field");
    m_hasError = true;
    m_errorMessage = (char*)"Invalid field name requested";

    return NULL;
}


int SQLite3Table::getIntField(int nField)
{
    if (fieldIsNull(nField))
    {
        return -1;
    }
    else
    {
        return atoi(fieldValue(nField));
    }
}


int SQLite3Table::getIntField(const char* szField)
{
    if (fieldIsNull(szField))
    {
        return -1;
    }
    else
    {
        return atoi(fieldValue(szField));
    }
}


double SQLite3Table::getFloatField(int nField)
{
    if (fieldIsNull(nField))
    {
        return -1.0;
    }
    else
    {
        return atof(fieldValue(nField));
    }
}


double SQLite3Table::getFloatField(const char* szField)
{
    if (fieldIsNull(szField))
    {
        return -1.0;
    }
    else
    {
        return atof(fieldValue(szField));
    }
}


const char* SQLite3Table::getStringField(int nField)
{
    if (fieldIsNull(nField))
    {
        return NULL;
    }
    else
    {
        return fieldValue(nField);
    }
}


const char* SQLite3Table::getStringField(const char* szField)
{
    if (fieldIsNull(szField))
    {
        return NULL;
    }
    else
    {
        return fieldValue(szField);
    }
}


bool SQLite3Table::fieldIsNull(int nField)
{
    checkResults();
    return (fieldValue(nField) == 0);
}


bool SQLite3Table::fieldIsNull(const char* szField)
{
    checkResults();
    return (fieldValue(szField) == 0);
}


const char* SQLite3Table::fieldName(int nCol)
{
    checkResults();

    if (nCol < 0 || nCol > m_nCols-1)
    {
        LOGE("fieldName Invalid field index requested");
        m_hasError = true;
        m_errorMessage = (char*)"Invalid field index requested";

        return NULL;
    }

    return m_paszResults[nCol];
}


void SQLite3Table::setRow(int nRow)
{
    checkResults();

    if (nRow < 0 || nRow > m_nRows-1)
    {
        LOGE("setRow set row error");
        m_hasError = true;
        m_errorMessage = (char*)"Invalid row index requested";

        return;
    }

    m_nCurrentRow = nRow;
}


void SQLite3Table::checkResults()
{
    if (m_paszResults == 0)
    {
        LOGE(" check result result");
        m_hasError = true;
        m_errorMessage = (char*)"Null Results pointer";
    }
}

v8::Handle<v8::Value> SQLite3Table::SQLite3Table::wrap()
{
    v8::HandleScope handle_scope;

    v8::Handle<v8::ObjectTemplate>              class_proto;
    v8::Handle<v8::Object>                      class_obj;
    v8::Handle<v8::FunctionTemplate>            class_func_templ = FunctionTemplate::New();

    class_func_templ->SetClassName(v8::String::New("SQLite3Table"));
    class_proto = class_func_templ->PrototypeTemplate();

    class_proto->Set("fieldName", FunctionTemplate::New(SQLite3Table::FieldName));
    class_proto->Set("numFields", FunctionTemplate::New(SQLite3Table::NumFields));
    class_proto->Set("fieldValue", FunctionTemplate::New(SQLite3Table::FieldValue));
    class_proto->Set("getIntField", FunctionTemplate::New(SQLite3Table::GetIntField));
    class_proto->Set("getFloatField", FunctionTemplate::New(SQLite3Table::GetFloatField));
    class_proto->Set("getStringField", FunctionTemplate::New(SQLite3Table::GetStringField));
    class_proto->Set("fieldIsNull", FunctionTemplate::New(SQLite3Table::FieldIsNull));
    class_proto->Set("numRows", FunctionTemplate::New(SQLite3Table::NumRows));
    class_proto->Set("setRow", FunctionTemplate::New(SQLite3Table::SetRow));
    class_proto->Set("finalize", FunctionTemplate::New(SQLite3Table::Finalize));

    //all class functions
    //class_proto->Set(v8::String::New("fillRect"),FunctionTemplate::New(GraphicsContext::fillRect));

    Handle<ObjectTemplate> class_ins = class_func_templ->InstanceTemplate();
    class_ins-> SetInternalFieldCount(1);
    //varables
    //class_ins->SetAccessor(v8::String::New("fillStyle"), AudioIns::GetFillStyle, AudioIns::SetFillStyle);

    //Handle<Function> class_ctor = class_func_templ ->GetFunction();
    //class_obj	= class_ctor ->NewInstance();
    class_obj = class_ins->NewInstance();
    class_obj->SetInternalField(0, External::New(this));

    return handle_scope.Close(class_obj);
}

v8::Handle<v8::Value> SQLite3Table::FieldName(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 1 || !args[0]->IsInt32())
        return THROW_EXCEPTION(TError, "SQLite3Query::FieldName : arguments Exception");

    SQLite3Table* table = ObjectWrap::Unwrap<SQLite3Table>(args.This());
    int col = args[0]->Int32Value();
    const char * res = table->fieldName(col);
    if(table->m_hasError)
    {
        table->m_hasError = false;
        return THROW_EXCEPTION(TError, table->m_errorMessage);
    }
    return v8::String::New(res);
}

v8::Handle<v8::Value> SQLite3Table::NumFields(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 0)
        return THROW_EXCEPTION(TError, "SQLite3Query::NumFields : arguments Exception");

    SQLite3Table* table = ObjectWrap::Unwrap<SQLite3Table>(args.This());
    int res = table->numFields();
    if(table->m_hasError)
    {
        table->m_hasError = false;
        return THROW_EXCEPTION(TError, table->m_errorMessage);
    }
    return v8::Int32::New(res);
}

v8::Handle<v8::Value> SQLite3Table::FieldValue(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 1)
        return THROW_EXCEPTION(TError, "SQLite3Query::FieldValue : arguments Exception");

    SQLite3Table* table = ObjectWrap::Unwrap<SQLite3Table>(args.This());
    if(args[0]->IsInt32())
    {
        int x = args[0]->Int32Value();
        char* res = (char*)table->fieldValue(x);
        if(table->m_hasError)
        {
            table->m_hasError = false;
            return THROW_EXCEPTION(TError, table->m_errorMessage);
        }

        return v8::String::New(res);
    }
    else if(args[0]->IsString())
    {
        v8::String::Utf8Value value(args[0]->ToString());
        const char * res = table->fieldValue(*value);
        if(table->m_hasError)
        {
            table->m_hasError = false;
            return THROW_EXCEPTION(TError, table->m_errorMessage);
        }
        return v8::String::New(res);
    }
    else
        return THROW_EXCEPTION(TError, "SQLite3Query::ExecQuery : arguments Exception");
}

v8::Handle<v8::Value> SQLite3Table::GetIntField(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 1)
        return THROW_EXCEPTION(TError, "SQLite3Query::GetIntField : arguments Exception");

    SQLite3Table* table = ObjectWrap::Unwrap<SQLite3Table>(args.This());
    int res;
    if(args[0]->IsInt32())
    {
        int x = args[0]->Int32Value();
        res = table->getIntField(x);
        if(table->m_hasError)
        {
            table->m_hasError = false;
            return THROW_EXCEPTION(TError, table->m_errorMessage);
        }
        return v8::Int32::New(res);
    }
    else if(args[0]->IsString())
    {
        v8::String::Utf8Value value(args[0]->ToString());
        res = table->getIntField(*value);
        if(table->m_hasError)
        {
            table->m_hasError = false;
            return THROW_EXCEPTION(TError, table->m_errorMessage);
        }
        return v8::Int32::New(res);
    }
    else
        return THROW_EXCEPTION(TError, "SQLite3Query::ExecQuery : arguments Exception");
}

v8::Handle<v8::Value> SQLite3Table::GetFloatField(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 1)
        return THROW_EXCEPTION(TError, "SQLite3Query::GetFloatField : arguments Exception");

    SQLite3Table* table = ObjectWrap::Unwrap<SQLite3Table>(args.This());
    float res = -1.0;
    if(args[0]->IsInt32())
    {
        int x = args[0]->Int32Value();
        res = table->getFloatField(x);
        if(table->m_hasError)
        {
            table->m_hasError = false;
            return THROW_EXCEPTION(TError, table->m_errorMessage);
        }
        return v8::Number::New(res);
    }
    else if(args[0]->IsString())
    {
        v8::String::Utf8Value value(args[0]->ToString());
        res = table->getFloatField(*value);
        if(table->m_hasError)
        {
            table->m_hasError = false;
            return THROW_EXCEPTION(TError, table->m_errorMessage);
        }
        return v8::Number::New(res);
    }
    else
        return THROW_EXCEPTION(TError, "SQLite3Query::ExecQuery : arguments Exception");
}

v8::Handle<v8::Value> SQLite3Table::GetStringField(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 1)
        return THROW_EXCEPTION(TError, "SQLite3Query::GetStringField : arguments Exception");

    SQLite3Table* table = ObjectWrap::Unwrap<SQLite3Table>(args.This());
    const char * res = NULL;
    if(args[0]->IsInt32())
    {
        int x = args[0]->Int32Value();
        res = table->getStringField(x);
        if(table->m_hasError)
        {
            table->m_hasError = false;
            return THROW_EXCEPTION(TError, table->m_errorMessage);
        }
        return v8::String::New(res);
    }
    else if(args[0]->IsString())
    {
        v8::String::Utf8Value value(args[0]->ToString());
        res = table->getStringField(*value);
        if(table->m_hasError)
        {
            table->m_hasError = false;
            return THROW_EXCEPTION(TError, table->m_errorMessage);
        }
        return v8::String::New(res);
    }
    else
        return THROW_EXCEPTION(TError, "SQLite3Query::ExecQuery : arguments Exception");
}

v8::Handle<v8::Value> SQLite3Table::FieldIsNull(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 1)
        return THROW_EXCEPTION(TError, "SQLite3Query::FieldIsNull : arguments Exception");

    SQLite3Table* table = ObjectWrap::Unwrap<SQLite3Table>(args.This());
    bool res = false;
    if(args[0]->IsInt32())
    {
        int x = args[0]->Int32Value();
        res = table->fieldIsNull(x);
        if(table->m_hasError)
        {
            table->m_hasError = false;
            return THROW_EXCEPTION(TError, table->m_errorMessage);
        }
        return v8::Boolean::New(res);
    }
    else if(args[0]->IsString())
    {
        v8::String::Utf8Value value(args[0]->ToString());
        res = table->fieldIsNull(*value);
        if(table->m_hasError)
        {
            table->m_hasError = false;
            return THROW_EXCEPTION(TError, table->m_errorMessage);
        }
        return v8::Boolean::New(res);
    }
    else
        return THROW_EXCEPTION(TError, "SQLite3Query::FieldIsNull : arguments Exception");
}

v8::Handle<v8::Value> SQLite3Table::NumRows(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 0)
        return THROW_EXCEPTION(TError, "SQLite3Query::NumRows : arguments Exception");

    SQLite3Table* table = ObjectWrap::Unwrap<SQLite3Table>(args.This());
    int res = table->numRows();
    if(table->m_hasError)
    {
        table->m_hasError = false;
        return THROW_EXCEPTION(TError, table->m_errorMessage);
    }
    return v8::Int32::New(res);
}

v8::Handle<v8::Value> SQLite3Table::SetRow(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 1)
        return THROW_EXCEPTION(TError, "SQLite3Query::SetRow : arguments Exception");

    SQLite3Table* table = ObjectWrap::Unwrap<SQLite3Table>(args.This());
    int nRow = args[0]->Int32Value();
    table->setRow(nRow);
    if(table->m_hasError)
    {
        table->m_hasError = false;
        return THROW_EXCEPTION(TError, table->m_errorMessage);
    }
    return v8::Undefined();
}

v8::Handle<v8::Value> SQLite3Table::Finalize(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 0)
        return THROW_EXCEPTION(TError, "SQLite3Query::Finalize : arguments Exception");
    SQLite3Table* table = ObjectWrap::Unwrap<SQLite3Table>(args.This());
    table->finalize();
    if(table->m_hasError)
    {
        table->m_hasError = false;
        return THROW_EXCEPTION(TError, table->m_errorMessage);
    }
    return v8::Undefined();
}

} // namespace DCanvas
