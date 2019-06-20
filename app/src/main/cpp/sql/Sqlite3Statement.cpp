#include "Sqlite3Statement.h"
#include "defines.h"

namespace DCanvas
{

SQLite3Statement::SQLite3Statement()
{
    m_pDB = 0;
    m_pVM = 0;
}

SQLite3Statement::SQLite3Statement(const SQLite3Statement& rStatement)
{
    m_pDB = rStatement.m_pDB;
    m_pVM = rStatement.m_pVM;
    // Only one object can own VM
    const_cast<SQLite3Statement&>(rStatement).m_pVM = 0;
}

SQLite3Statement::SQLite3Statement(sqlite3* pDB, sqlite3_stmt* pVM)
{
    m_pDB = pDB;
    m_pVM = pVM;
}

SQLite3Statement::~SQLite3Statement()
{
    if(-1 == finalize()){}
}

SQLite3Statement& SQLite3Statement::operator=(const SQLite3Statement& rStatement)
{
    m_pDB = rStatement.m_pDB;
    m_pVM = rStatement.m_pVM;
    // Only one object can own VM
    const_cast<SQLite3Statement&>(rStatement).m_pVM = 0;
    return *this;
}

int SQLite3Statement::execDML()
{
    checkDB();
    checkVM();

    const char* szError=0;

    int nRet = sqlite3_step(m_pVM);

    if (nRet == SQLITE_DONE)
    {
        int nRowsChanged = sqlite3_changes(m_pDB);

        nRet = sqlite3_reset(m_pVM);

        if (nRet != SQLITE_OK)
        {
            szError = sqlite3_errmsg(m_pDB);
            //throw CSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
        }

        return nRowsChanged;
    }
    else
    {
        nRet = sqlite3_reset(m_pVM);
        szError = sqlite3_errmsg(m_pDB);
        //throw CSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
    }
    return -1;
}

SQLite3Query* SQLite3Statement::execQuery()
{
    checkDB();
    checkVM();

    int nRet = sqlite3_step(m_pVM);

    if (nRet == SQLITE_DONE)
    {
        // no rows
        return new SQLite3Query(m_pDB, m_pVM, true/*eof*/, false);
    }
    else if (nRet == SQLITE_ROW)
    {
        // at least 1 row
        return new SQLite3Query(m_pDB, m_pVM, false/*eof*/, false);
    }
    else
    {
        nRet = sqlite3_reset(m_pVM);
        const char* szError = sqlite3_errmsg(m_pDB);
        //throw CSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
        return NULL;//SQLite3Query(m_pDB, m_pVM, true/*eof*/, false);
    }
}

void SQLite3Statement::bind(int nParam, const char* szValue)
{
    checkVM();
    int nRes = sqlite3_bind_text(m_pVM, nParam, szValue, -1, SQLITE_TRANSIENT);

    if (nRes != SQLITE_OK)
    {
        //throw CSQLite3Exception(nRes, "Error binding string param", DONT_DELETE_MSG);
    }
}

void SQLite3Statement::bind(int nParam, const int nValue)
{
    checkVM();
    int nRes = sqlite3_bind_int(m_pVM, nParam, nValue);

    if (nRes != SQLITE_OK)
    {
        //throw CSQLite3Exception(nRes, "Error binding int param", DONT_DELETE_MSG);
    }
}

void SQLite3Statement::bind(int nParam, const double dValue)
{
    checkVM();
    int nRes = sqlite3_bind_double(m_pVM, nParam, dValue);

    if (nRes != SQLITE_OK)
    {
        //throw CSQLite3Exception(nRes, "Error binding double param", DONT_DELETE_MSG);
    }
}

void SQLite3Statement::bind(int nParam, const unsigned char* blobValue, int nLen)
{
    checkVM();
    int nRes = sqlite3_bind_blob(m_pVM, nParam,
                                (const void*)blobValue, nLen, SQLITE_TRANSIENT);

    if (nRes != SQLITE_OK)
    {
        //throw CSQLite3Exception(nRes, "Error binding blob param", DONT_DELETE_MSG);
    }
}

void SQLite3Statement::bindNull(int nParam)
{
    checkVM();
    int nRes = sqlite3_bind_null(m_pVM, nParam);

    if (nRes != SQLITE_OK)
    {
        //throw CSQLite3Exception(nRes, "Error binding NULL param", DONT_DELETE_MSG);
    }
}

void SQLite3Statement::reset()
{
    if (m_pVM)
    {
        int nRet = sqlite3_reset(m_pVM);

        if (nRet != SQLITE_OK)
        {
            const char* szError = sqlite3_errmsg(m_pDB);
            //throw CSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
        }
    }
}

int SQLite3Statement::finalize()
{
    if (m_pVM)
    {
        int nRet = sqlite3_finalize(m_pVM);
        m_pVM = 0;

        if (nRet != SQLITE_OK)
        {
            const char* szError = sqlite3_errmsg(m_pDB);
            //throw CSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
            return -1;
        }
    }
    return 0;
}

void SQLite3Statement::checkDB()
{
    if (m_pDB == 0)
    {
        //throw CSQLite3Exception(CPPSQLITE_ERROR, "Database not open", DONT_DELETE_MSG);
    }
}

void SQLite3Statement::checkVM()
{
    if (m_pVM == 0)
    {
        //throw CSQLite3Exception(CPPSQLITE_ERROR, "Null Virtual Machine pointer", DONT_DELETE_MSG);
    }
}

v8::Handle<v8::Value> SQLite3Statement::wrap()
{
    v8::HandleScope handle_scope;

    v8::Handle<v8::ObjectTemplate>              class_proto;
    v8::Handle<v8::Object>                      class_obj;
    v8::Handle<v8::FunctionTemplate>            class_func_templ = FunctionTemplate::New();

    class_func_templ->SetClassName(v8::String::New("SQLite3Statement"));
    class_proto = class_func_templ->PrototypeTemplate();

    class_proto->Set("execDML", FunctionTemplate::New(SQLite3Statement::ExecDML));
    class_proto->Set("execQuery", FunctionTemplate::New(SQLite3Statement::ExecQuery));
    class_proto->Set("bind", FunctionTemplate::New(SQLite3Statement::Bind));
    class_proto->Set("bindNull", FunctionTemplate::New(SQLite3Statement::BindNull));
    class_proto->Set("reset", FunctionTemplate::New(SQLite3Statement::Reset));
    class_proto->Set("finalize", FunctionTemplate::New(SQLite3Statement::Finalize));
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

v8::Handle<v8::Value> SQLite3Statement::ExecDML(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 0)
        return THROW_EXCEPTION(TError, "SQLite3Statement::ExecDML : arguments Exception");

    SQLite3Statement* stmt = ObjectWrap::Unwrap<SQLite3Statement>(args.This());

    stmt->execDML();
    return v8::Undefined();
}

v8::Handle<v8::Value> SQLite3Statement::ExecQuery(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 0)
        return THROW_EXCEPTION(TError, "SQLite3Statement::ExecQuery : arguments Exception");

    SQLite3Statement* stmt = ObjectWrap::Unwrap<SQLite3Statement>(args.This());

    SQLite3Query* query = stmt->execQuery();
    return query->wrap();
}

v8::Handle<v8::Value> SQLite3Statement::Bind(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    SQLite3Statement* stmt = ObjectWrap::Unwrap<SQLite3Statement>(args.This());

    if(args.Length() == 2)
    {
        if(args[0]->IsInt32())
        {
            int pa = args[0]->Int32Value();
            if(args[1]->IsString())
            {
                v8::String::Utf8Value value(args[1]->ToString());
                stmt->bind(pa, *value);
            }
            else if(args[1]->IsInt32())
            {
                int nvalue = args[1]->Int32Value();
                stmt->bind(pa, nvalue);
            }
            else if(args[1]->IsNumber())
            {
                float f = args[1]->NumberValue();
                stmt->bind(pa, f);
            }
            else
                return THROW_EXCEPTION(TError, "SQLite3Statement::Bind : arguments Exception");
        }
        else
            return THROW_EXCEPTION(TError, "SQLite3Statement::Bind : arguments Exception");
    }
    else
        return THROW_EXCEPTION(TError, "SQLite3Statement::Bind : arguments Exception");

    return v8::Undefined();
}

v8::Handle<v8::Value> SQLite3Statement::BindNull(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 1 || ! args[0]->IsInt32())
        return THROW_EXCEPTION(TError, "SQLite3Statement::BindNull : arguments Exception");

    SQLite3Statement* stmt = ObjectWrap::Unwrap<SQLite3Statement>(args.This());

    int param = args[0]->Int32Value();
    stmt->bindNull(param);
    return v8::Undefined();
}

v8::Handle<v8::Value> SQLite3Statement::Reset(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 0)
        return THROW_EXCEPTION(TError, "SQLite3Statement::Reset : arguments Exception");

    SQLite3Statement* stmt = ObjectWrap::Unwrap<SQLite3Statement>(args.This());

    stmt->reset();
    return v8::Undefined();
}

v8::Handle<v8::Value> SQLite3Statement::Finalize(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 0)
        return THROW_EXCEPTION(TError, " SQLite3Statement::Finalize : arguments Exception");

    SQLite3Statement* stmt = ObjectWrap::Unwrap<SQLite3Statement>(args.This());

    stmt->finalize();
    return v8::Undefined();
}
}
