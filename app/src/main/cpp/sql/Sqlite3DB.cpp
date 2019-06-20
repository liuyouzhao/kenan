#include "defines.h"
#include "Sqlite3DB.h"
#include <stdio.h>
#include <stdlib.h>

#undef LOG_TAG
#define  LOG_TAG    "Sqlite3DB"

namespace DCanvas
{

SQLite3DB::SQLite3DB()
{
    m_pDB = 0;
    m_nBusyTimeoutMs = 60000; // 60 seconds
    open("/data/data/com.aliyun.cloudapp.directcanvas/canvasDB.db");
}


SQLite3DB::SQLite3DB(const SQLite3DB& db)
{
    m_pDB = db.m_pDB;
    m_nBusyTimeoutMs = 60000; // 60 seconds
}


SQLite3DB::~SQLite3DB()
{
    close();
}


SQLite3DB& SQLite3DB::operator=(const SQLite3DB& db)
{
    m_pDB = db.m_pDB;
    m_nBusyTimeoutMs = 60000; // 60 seconds
    return *this;
}


void SQLite3DB::open(const char* szFile)
{
    int nRet = sqlite3_open(szFile, &m_pDB);

    if (nRet != SQLITE_OK)
    {
        const char* szError = sqlite3_errmsg(m_pDB);
        LOGE("%s", szError);
        return;//throw CSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
    }else LOGD("db open");

    setBusyTimeout(m_nBusyTimeoutMs);
}


void SQLite3DB::close()
{
    if (m_pDB)
    {
        sqlite3_close(m_pDB);
        m_pDB = 0;
    }
}


SQLite3Statement* SQLite3DB::compileStatement(const char* szSQL)
{
    checkDB();

    sqlite3_stmt* pVM = compile(szSQL);
    return new SQLite3Statement(m_pDB, pVM);
}


bool SQLite3DB::tableExists(const char* szTable)
{
    char szSQL[128];
    sprintf(szSQL,
            "select count(*) from sqlite_master where type='table' and name='%s'",
            szTable);
    int nRet = execScalar(szSQL);

    return (nRet > 0);
}


int SQLite3DB::execDML(const char* szSQL)
{
    checkDB();

    char* szError=0;

    int nRet = sqlite3_exec(m_pDB, szSQL, 0, 0, &szError);

    if (nRet == SQLITE_OK)
    {
        return sqlite3_changes(m_pDB);
    }
    else
    {
        LOGE("SQLite3DB::execDML :: throw exception!");
        return -1;//throw CSQLite3Exception(nRet, szError);
    }
}


SQLite3Query* SQLite3DB::execQuery(const char* szSQL)
{
    checkDB();
    //control execute
    sqlite3_stmt* pVM = compile(szSQL);

    int nRet = sqlite3_step(pVM);

    if (nRet == SQLITE_DONE)
    {
        // no rows
        return new SQLite3Query(m_pDB, pVM, true/*eof*/);
    }
    else if (nRet == SQLITE_ROW)
    {
        // at least 1 row
        return new SQLite3Query(m_pDB, pVM, false/*eof*/);
    }
    else
    {
        nRet = sqlite3_finalize(pVM);
        const char* szError= sqlite3_errmsg(m_pDB);
        //throw CSQLite3Exception(nRet, (char*)szError, DONT_DELETE_MSG);
        LOGE("throw exception :: SQLite3DB::execQuery");
        return NULL;
    }
}


int SQLite3DB::execScalar(const char* szSQL)
{
    SQLite3Query* q = execQuery(szSQL);

    if (q->eof() || q->numFields() < 1)
    {
        //throw CSQLite3Exception(CPPSQLITE_ERROR, "Invalid scalar query", DONT_DELETE_MSG);
        LOGE("throw exception :: SQLite3DB::execScalar");

        return -1;
    }
    int res = atoi(q->fieldValue(0));
    delete q;
    return res;
}


SQLite3Table* SQLite3DB::getTable(const char* szSQL)
{
    checkDB();

    char* szError=0;
    char** paszResults=0;
    int nRet;
    int nRows(0);
    int nCols(0);

    nRet = sqlite3_get_table(m_pDB, szSQL, &paszResults, &nRows, &nCols, &szError);

    if (nRet == SQLITE_OK)
    {
        return new SQLite3Table(paszResults, nRows, nCols);
    }
    else
    {
        //throw CSQLite3Exception(nRet, szError);
        return NULL;
    }
}


sqlite_int64 SQLite3DB::lastRowId()
{
    return sqlite3_last_insert_rowid(m_pDB);
}


void SQLite3DB::setBusyTimeout(int nMillisecs)
{
    m_nBusyTimeoutMs = nMillisecs;
    sqlite3_busy_timeout(m_pDB, m_nBusyTimeoutMs);
}


void SQLite3DB::checkDB()
{
    if (!m_pDB)
    {
        //throw CSQLite3Exception(CPPSQLITE_ERROR, "Database not open", DONT_DELETE_MSG);
    }
}


sqlite3_stmt* SQLite3DB::compile(const char* szSQL)
{
    checkDB();

    char* szError=0;
    const char* szTail=0;
    sqlite3_stmt* pVM;

    int nRet = sqlite3_prepare(m_pDB, szSQL, -1, &pVM, &szTail);

    if (nRet != SQLITE_OK)
    {
        //throw CSQLite3Exception(nRet, szError);
        return NULL;
    }

    return pVM;
}

v8::Handle<v8::Value> SQLite3DB::ExecDML(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 1 || !args[0]->IsString())
        return THROW_EXCEPTION(TError, "SQLite3DB::ExecDML : arguments Exception!!");

    SQLite3DB* db = ObjectWrap::Unwrap<SQLite3DB>(args.This());	
    v8::String::Utf8Value value(args[0]->ToString());

    return v8::Int32::New(db->execDML(*value));
}

v8::Handle<v8::Value> SQLite3DB::SQLiteVersion(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 0)
        return THROW_EXCEPTION(TError, "SQLite3DB::SQLiteVersion : arguments Exception!!");

    SQLite3DB* db = ObjectWrap::Unwrap<SQLite3DB>(args.This());

    return v8::String::New(db->SQLiteVersion());
}

v8::Handle<v8::Value> SQLite3DB::CompileStatement(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 1 || !args[0]->IsString())
        return THROW_EXCEPTION(TError, "SQLite3DB::CompileStatement : arguments Exception!!");

    SQLite3DB* db = ObjectWrap::Unwrap<SQLite3DB>(args.This());
    v8::String::Utf8Value value(args[0]->ToString());
    SQLite3Statement* stat = db->compileStatement(*value);
    return stat->wrap();
}

v8::Handle<v8::Value> SQLite3DB::GetTable(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 1 || !args[0]->IsString())
        return THROW_EXCEPTION(TError, "SQLite3DB::GetTable : arguments Exception!!");

    SQLite3DB* db = ObjectWrap::Unwrap<SQLite3DB>(args.This());
    v8::String::Utf8Value value(args[0]->ToString());
    SQLite3Table* table = db->getTable(*value);
    if(table == NULL)
        return THROW_EXCEPTION(TError, "SQLite3DB::GetTable : arguments Exception!!");

    return table->wrap();
}

v8::Handle<v8::Value> SQLite3DB::ExecQuery(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 1 || !args[0]->IsString())
        return THROW_EXCEPTION(TError, "SQLite3DB::ExecQuery : arguments Exception!!");

    SQLite3DB* db = ObjectWrap::Unwrap<SQLite3DB>(args.This());
    v8::String::Utf8Value value(args[0]->ToString());
    SQLite3Query* query = db->execQuery(*value);

    return query->wrap();
}

v8::Handle<v8::Value> SQLite3DB::TableExists(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 1 || !args[0]->IsString())
        return THROW_EXCEPTION(TError, "SQLite3DB::TableExists : arguments Exception!!");

    SQLite3DB* db = ObjectWrap::Unwrap<SQLite3DB>(args.This());
    v8::String::Utf8Value value(args[0]->ToString());

    return v8::Boolean::New(db->tableExists(*value));
}

v8::Handle<v8::Value> SQLite3DB::ExecScalar(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 1 || !args[0]->IsString())
        return THROW_EXCEPTION(TError, "SQLite3DB::ExecScalar : arguments Exception!!");

    SQLite3DB* db = ObjectWrap::Unwrap<SQLite3DB>(args.This());
    v8::String::Utf8Value value(args[0]->ToString());

    return v8::Int32::New(db->execScalar(*value));
}

v8::Handle<v8::Value> SQLite3DB::LastRowId(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    HandleScope scope;
    if(args.Length() != 0)
        return THROW_EXCEPTION(TError, "SQLite3DB::LastRowId : arguments Exception!!");
    SQLite3DB* db = ObjectWrap::Unwrap<SQLite3DB>(args.This());

    return v8::Int32::New(db->lastRowId());
}

v8::Handle<v8::Value> SQLite3DB::wrap()
{
    v8::HandleScope handle_scope;

    v8::Handle<v8::ObjectTemplate>              class_proto;
    v8::Handle<v8::Object>                      class_obj;
    v8::Handle<v8::FunctionTemplate>            class_func_templ = FunctionTemplate::New();

    class_func_templ->SetClassName(v8::String::New("SQLite3DB"));
    class_proto = class_func_templ->PrototypeTemplate();

    class_proto->Set("execDML", FunctionTemplate::New(SQLite3DB::ExecDML));
    class_proto->Set("Compile", FunctionTemplate::New(SQLite3DB::CompileStatement));
    class_proto->Set("getTable", FunctionTemplate::New(SQLite3DB::GetTable));
    class_proto->Set("execQuery", FunctionTemplate::New(SQLite3DB::ExecQuery));
    class_proto->Set("tableExists", FunctionTemplate::New(SQLite3DB::TableExists));
    class_proto->Set("version", FunctionTemplate::New(SQLite3DB::SQLiteVersion));
    class_proto->Set("execScalar", FunctionTemplate::New(SQLite3DB::ExecScalar));
    class_proto->Set("lastRowId", FunctionTemplate::New(SQLite3DB::LastRowId));

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

}// namepace DCanvas
