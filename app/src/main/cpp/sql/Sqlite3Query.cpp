#include "defines.h"
#include "Sqlite3Query.h"
#include <stdlib.h>
#include <string.h>
#include "V8ArrayType.h"
#include "Uint8Array.h"

#undef LOG_TAG
#define  LOG_TAG    "SQLite3Query"

namespace DCanvas
{

//must finalize
SQLite3Query::SQLite3Query()
{
    m_pVM = 0;
    m_bEof = true;
    m_nCols = 0;
    m_bOwnVM = false;
    m_hasError = false;
}


SQLite3Query::SQLite3Query(const SQLite3Query& rQuery)
{

    m_pVM = rQuery.m_pVM;
    // Only one object can own the VM
    const_cast<SQLite3Query&>(rQuery).m_pVM = 0;
    m_bEof = rQuery.m_bEof;
    m_nCols = rQuery.m_nCols;
    m_bOwnVM = rQuery.m_bOwnVM;
    m_hasError = false;
}


SQLite3Query::SQLite3Query(sqlite3* pDB,
                            sqlite3_stmt* pVM,
                            bool bEof,
                            bool bOwnVM/*=true*/)
{
    m_pDB = pDB;
    m_pVM = pVM;
    m_bEof = bEof;
    m_nCols = sqlite3_column_count(m_pVM);
    m_bOwnVM = bOwnVM;
    m_hasError = false;
}


SQLite3Query::~SQLite3Query()
{
    if(-1 == finalize())
    {
    }
}


SQLite3Query& SQLite3Query::operator=(const SQLite3Query& rQuery)
{
    if(-1 == finalize())
    {
    }
    m_pVM = rQuery.m_pVM;
    // Only one object can own the VM
    const_cast<SQLite3Query&>(rQuery).m_pVM = 0;
    m_bEof = rQuery.m_bEof;
    m_nCols = rQuery.m_nCols;
    m_bOwnVM = rQuery.m_bOwnVM;

    return *this;
}


int SQLite3Query::numFields()
{
    checkVM();
    return m_nCols;
}


const char* SQLite3Query::fieldValue(int nField)
{
    checkVM();

    if (nField < 0 || nField > m_nCols-1)
    {
        LOGE("fieldValue has error");
        m_hasError = true;
        m_errorMessage = (char*)"Invalid field index requested";
        return NULL;
    }

    return (const char*)sqlite3_column_text(m_pVM, nField);
}


const char* SQLite3Query::fieldValue(const char* szField)
{
    int nField = fieldIndex(szField);
    if(nField == -1)
        return NULL;
    return (const char*)sqlite3_column_text(m_pVM, nField);
}


int SQLite3Query::getIntField(int nField)
{
    if (fieldDataType(nField) == SQLITE_NULL)
    {
        return -1;
    }
    else
    {
        return sqlite3_column_int(m_pVM, nField);
    }
}


int SQLite3Query::getIntField(const char* szField)
{
    int nField = fieldIndex(szField);
    if(nField == -1)
        return -1;
    return getIntField(nField);
}


double SQLite3Query::getFloatField(int nField)
{
    if (fieldDataType(nField) == SQLITE_NULL)
    {
        return -1.0;
    }
    else
    {
        return sqlite3_column_double(m_pVM, nField);
    }
}


double SQLite3Query::getFloatField(const char* szField)
{
	int nField = fieldIndex(szField);
    if(nField == -1.0)
        return -1.0;
	return getFloatField(nField);
}


const char* SQLite3Query::getStringField(int nField)
{
    if (fieldDataType(nField) == SQLITE_NULL)
    {
        return NULL;
    }
    else
    {
        return (const char*)sqlite3_column_text(m_pVM, nField);
    }
}


const char* SQLite3Query::getStringField(const char* szField)
{
    int nField = fieldIndex(szField);
    if(nField == -1)
        return NULL;
    return getStringField(nField);
}


const unsigned char* SQLite3Query::getBlobField(int nField, int& nLen)
{
    checkVM();

    if (nField < 0 || nField > m_nCols-1)
    {
        LOGE("getBlobField has error");
        m_hasError = true;
        m_errorMessage = (char*)"Invalid field index requested";
        return NULL;
    }

    nLen = sqlite3_column_bytes(m_pVM, nField);
    return (const unsigned char*)sqlite3_column_blob(m_pVM, nField);
}


const unsigned char* SQLite3Query::getBlobField(const char* szField, int& nLen)
{
    int nField = fieldIndex(szField);
    if(nField == -1)
        return NULL;
    return getBlobField(nField, nLen);
}


bool SQLite3Query::fieldIsNull(int nField)
{
    return (fieldDataType(nField) == SQLITE_NULL);
}


bool SQLite3Query::fieldIsNull(const char* szField)
{
    int nField = fieldIndex(szField);
    if(nField == -1)
        return true;
    return (fieldDataType(nField) == SQLITE_NULL);
}

int SQLite3Query::fieldIndex(const char* szField)
{
    checkVM();

    if (szField)
    {
        for (int nField = 0; nField < m_nCols; nField++)
        {
            const char* szTemp = sqlite3_column_name(m_pVM, nField);

            if (strcmp(szField, szTemp) == 0)
            {
                return nField;
            }
        }
    }
    m_hasError = true;
    m_errorMessage = (char*)"Invalid field name requested";
    return -1;
}


const char* SQLite3Query::fieldName(int nCol)
{
    checkVM();

    if (nCol < 0 || nCol > m_nCols-1)
    {
        LOGE("fieldname error");
        m_hasError = true;
        m_errorMessage = (char*)"Invalid field name requested";
        return NULL;
    }

    return sqlite3_column_name(m_pVM, nCol);
}


const char* SQLite3Query::fieldDeclType(int nCol)
{
    checkVM();

    if (nCol < 0 || nCol > m_nCols-1)
    {
        LOGE("fieldDeclType error");
        m_hasError = true;
        m_errorMessage = (char*)"Invalid field index requested";
        return "unknown";
    }

    return sqlite3_column_decltype(m_pVM, nCol);
}


int SQLite3Query::fieldDataType(int nCol)
{
    checkVM();

    if (nCol < 0 || nCol > m_nCols-1)
    {
        LOGE("fieldDataType error");
        m_hasError = true;
        m_errorMessage = (char*)"Invalid field index requested";
        return -1;
    }

    return sqlite3_column_type(m_pVM, nCol);
}


bool SQLite3Query::eof()
{
    checkVM();
    return m_bEof;
}


void SQLite3Query::nextRow()
{
    checkVM();

    int nRet = sqlite3_step(m_pVM);

    if (nRet == SQLITE_DONE)
    {
        // no rows
        m_bEof = true;
    }
    else if (nRet == SQLITE_ROW)
    {
        // more rows, nothing to do
    }
    else
    {
        nRet = sqlite3_finalize(m_pVM);
        m_pVM = 0;
        const char* szError = sqlite3_errmsg(m_pDB);
        LOGE("nextRow error");
        m_hasError = true;
        m_errorMessage = (char*)szError;
    }
}


int SQLite3Query::finalize()
{
    if (m_pVM && m_bOwnVM)
    {
        int nRet = sqlite3_finalize(m_pVM);
        m_pVM = 0;
        if (nRet != SQLITE_OK)
        {
            const char* szError = sqlite3_errmsg(m_pDB);
            LOGE("finalize error");
            m_hasError = true;
            m_errorMessage = (char*)szError;
            return -1;
        }
    }
    return 0;
}


void SQLite3Query::checkVM()
{
    if (m_pVM == 0)
    {
        LOGE("finalize error");
        m_hasError = true;
        m_errorMessage = (char*)"Null Virtual Machine pointer";

    }
}

v8::Handle<v8::Value> SQLite3Query::wrap()
{
    v8::HandleScope handle_scope;

    v8::Handle<v8::ObjectTemplate>					class_proto;
    v8::Handle<v8::Object>							class_obj;
    v8::Handle<v8::FunctionTemplate>				class_func_templ = FunctionTemplate::New();

    class_func_templ->SetClassName(v8::String::New("SQLite3Query"));
    class_proto = class_func_templ->PrototypeTemplate();

    class_proto->Set("numFields", FunctionTemplate::New(SQLite3Query::NumFields));
    class_proto->Set("fieldName", FunctionTemplate::New(SQLite3Query::FieldName));
    class_proto->Set("fieldIndex", FunctionTemplate::New(SQLite3Query::FieldIndex));
    class_proto->Set("fieldValue", FunctionTemplate::New(SQLite3Query::FieldValue));
    class_proto->Set("getIntField", FunctionTemplate::New(SQLite3Query::GetIntField));
    class_proto->Set("getFloatField", FunctionTemplate::New(SQLite3Query::GetFloatField));
    class_proto->Set("getBlobField", FunctionTemplate::New(SQLite3Query::GetBlobField));
    class_proto->Set("fieldIsNull", FunctionTemplate::New(SQLite3Query::FieldIsNull));
    class_proto->Set("eof", FunctionTemplate::New(SQLite3Query::Eof));
    class_proto->Set("nextRow", FunctionTemplate::New(SQLite3Query::NextRow));
    class_proto->Set("finalize", FunctionTemplate::New(SQLite3Query::Finalize));
    class_proto->Set("fieldDeclType", FunctionTemplate::New(SQLite3Query::FieldDeclType));

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

v8::Handle<v8::Value> SQLite3Query::NumFields(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 0)
        return THROW_EXCEPTION(TError, "SQLite3DB::NumFields : arguments Exception");

    SQLite3Query* query = ObjectWrap::Unwrap<SQLite3Query>(args.This());
    int res = query->numFields();
    if(query->m_hasError)
    {
        query->m_hasError = false;	
        return v8::ThrowException(v8::Exception::Error(v8::String::New(query->m_errorMessage)));
    }
    return v8::Int32::New(res);
}

v8::Handle<v8::Value> SQLite3Query::FieldName(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 1 || !args[0]->IsInt32())
        return THROW_EXCEPTION(TError, "SQLite3DB::FieldName : arguments Exception");

    SQLite3Query* query = ObjectWrap::Unwrap<SQLite3Query>(args.This());
    int col = args[0]->Int32Value();
    const char * res = query->fieldName(col);
    if(query->m_hasError)
    {
        query->m_hasError = false;	
        return THROW_EXCEPTION(TError, query->m_errorMessage);
    }
    return v8::String::New(res);
}

v8::Handle<v8::Value> SQLite3Query::FieldDeclType(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 1 || !args[0]->IsInt32())
        return THROW_EXCEPTION(TError, "SQLite3DB::FieldDeclType : arguments Exception");

    SQLite3Query* query = ObjectWrap::Unwrap<SQLite3Query>(args.This());
    int col = args[0]->Int32Value();

    const char * res = query->fieldDeclType(col);
    if(query->m_hasError)
    {
        query->m_hasError = false;	
        return THROW_EXCEPTION(TError, query->m_errorMessage);
    }
    return v8::String::New(res);
}

v8::Handle<v8::Value> SQLite3Query::FieldIndex(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 1 || !args[0]->IsString())
        return THROW_EXCEPTION(TError, "SQLite3DB::FieldIndex : arguments Exception");

    SQLite3Query* query = ObjectWrap::Unwrap<SQLite3Query>(args.This());
    v8::String::Utf8Value value(args[0]->ToString());
    int res =  query->fieldIndex(*value);
    if(query->m_hasError)
    {
        query->m_hasError = false;	
        return THROW_EXCEPTION(TError, query->m_errorMessage);
    }
    return v8::Int32::New(res);
}

v8::Handle<v8::Value> SQLite3Query::FieldValue(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 1)
        return THROW_EXCEPTION(TError, "SQLite3DB::FieldValue : arguments Exception");

    SQLite3Query* query = ObjectWrap::Unwrap<SQLite3Query>(args.This());
    if(args[0]->IsInt32())
    {
        int x = args[0]->Int32Value();
        const char* v = query->fieldValue(x);
        if(query->m_hasError)
        {
            query->m_hasError = false;	
            return THROW_EXCEPTION(TError, query->m_errorMessage);
        }
        if(v == NULL)
            LOGE("is null ??");
        return v8::String::New(v);
    }
    else if(args[0]->IsString())
    {
        v8::String::Utf8Value value(args[0]->ToString());
        const char * res = query->fieldValue(*value);
        if(query->m_hasError)
        {
            query->m_hasError = false;	
            return THROW_EXCEPTION(TError, query->m_errorMessage);
        }
        if(res == NULL)
            LOGE("why is null");
        return v8::String::New(res);
    }
    else
        return THROW_EXCEPTION(TError, "SQLite3DB::FieldValue : arguments Exception");
}

v8::Handle<v8::Value> SQLite3Query::GetIntField(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 1)
        return THROW_EXCEPTION(TError, "SQLite3DB::GetIntField : arguments Exception");

    SQLite3Query* query = ObjectWrap::Unwrap<SQLite3Query>(args.This());
    int res;
    if(args[0]->IsInt32())
    {
        int x = args[0]->Int32Value();
        res = query->getIntField(x);
        if(query->m_hasError)
        {
            query->m_hasError = false;	
            return THROW_EXCEPTION(TError, query->m_errorMessage);
        }
        return v8::Int32::New(res);
    }
    else if(args[0]->IsString())
    {
        v8::String::Utf8Value value(args[0]->ToString());
        res = query->getIntField(*value);
        if(query->m_hasError)
        {
            query->m_hasError = false;	
            return THROW_EXCEPTION(TError, query->m_errorMessage);
        }
        return v8::Int32::New(res);
    }
    else
        return THROW_EXCEPTION(TError, "SQLite3DB::GetIntField : arguments Exception");
}

v8::Handle<v8::Value> SQLite3Query::GetFloatField(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 1)
        return THROW_EXCEPTION(TError, "SQLite3DB::GetFloatField : arguments Exception");

    SQLite3Query* query = ObjectWrap::Unwrap<SQLite3Query>(args.This());
    float res = 0.0;
    if(args[0]->IsInt32())
    {
        int x = args[0]->Int32Value();
        res = query->getFloatField(x);
        if(query->m_hasError)
        {
            query->m_hasError = false;	
            return THROW_EXCEPTION(TError, query->m_errorMessage);
        }

        return v8::Number::New(res);
    }
    else if(args[0]->IsString())
    {
        v8::String::Utf8Value value(args[0]->ToString());
        res = query->getFloatField(*value);
        if(query->m_hasError)
        {
            query->m_hasError = false;	
            return THROW_EXCEPTION(TError, query->m_errorMessage);
        }
        return v8::Number::New(res);
    }
    else
        return THROW_EXCEPTION(TError, "SQLite3DB::GetFloatField : arguments Exception");
}

v8::Handle<v8::Value> SQLite3Query::GetBlobField(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 1)
        return THROW_EXCEPTION(TError, "SQLite3DB::GetBlobField : arguments Exception");

    SQLite3Query* query = ObjectWrap::Unwrap<SQLite3Query>(args.This());
    int nlen = 0;
    const unsigned char *res = NULL;
    if(args[0]->IsInt32())
    {
        int x = args[0]->Int32Value();
        res = query->getBlobField(x, nlen);
    }
    else if(args[0]->IsString())
    {
        v8::String::Utf8Value value(args[0]->ToString());
        res = query->getBlobField(*value, nlen);
    }
    else
        return THROW_EXCEPTION(TError, "SQLite3DB::GetBlobField : arguments Exception");

    if(query->m_hasError)
    {
        query->m_hasError = false;	
        return THROW_EXCEPTION(TError, query->m_errorMessage);
    }
    Uint8Array* array = Uint8Array::create(res, nlen);
    return V8ArrayType<Uint8Array>::Wrap(array);
}

v8::Handle<v8::Value> SQLite3Query::FieldIsNull(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 1)
        return THROW_EXCEPTION(TError, "SQLite3DB::FieldIsNull : arguments Exception");

    SQLite3Query* query = ObjectWrap::Unwrap<SQLite3Query>(args.This());
    bool res = false;
    if(args[0]->IsInt32())
    {
        int x = args[0]->Int32Value();
        res = query->fieldIsNull(x);
        if(query->m_hasError)
        {
            query->m_hasError = false;	
            return THROW_EXCEPTION(TError, query->m_errorMessage);
        }
        return v8::Boolean::New(res);
    }
    else if(args[0]->IsString())
    {
        v8::String::Utf8Value value(args[0]->ToString());
        res = query->fieldIsNull(*value);
        if(query->m_hasError)
        {
            query->m_hasError = false;	
            return THROW_EXCEPTION(TError, query->m_errorMessage);
        }
        return v8::Boolean::New(res);
    }
    else
        return THROW_EXCEPTION(TError, "SQLite3DB::FieldIsNull : arguments Exception");

}

v8::Handle<v8::Value> SQLite3Query::Eof(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 0)
        return THROW_EXCEPTION(TError, "SQLite3DB::Eof : arguments Exception");

    SQLite3Query* query = ObjectWrap::Unwrap<SQLite3Query>(args.This());
    bool res = query->eof();
    if(query->m_hasError)
    {
        query->m_hasError = false;	
        return THROW_EXCEPTION(TError, query->m_errorMessage);
    }
    return v8::Boolean::New(res);
}

v8::Handle<v8::Value> SQLite3Query::NextRow(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 0)
        return THROW_EXCEPTION(TError, "SQLite3DB::NextRow : arguments Exception");

    SQLite3Query* query = ObjectWrap::Unwrap<SQLite3Query>(args.This());
    query->nextRow();
    if(query->m_hasError)
    {
        query->m_hasError = false;	
        return THROW_EXCEPTION(TError, query->m_errorMessage);
    }
    return v8::Undefined();
}

v8::Handle<v8::Value> SQLite3Query::Finalize(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 0)
        return THROW_EXCEPTION(TError, "SQLite3DB::Finalize : arguments Exception");

    SQLite3Query* query = ObjectWrap::Unwrap<SQLite3Query>(args.This());
    query->finalize();
    if(query->m_hasError)
    {
        query->m_hasError = false;	
        return THROW_EXCEPTION(TError, query->m_errorMessage);
    }
    return v8::Undefined();
}

}// namespace DCanvas
