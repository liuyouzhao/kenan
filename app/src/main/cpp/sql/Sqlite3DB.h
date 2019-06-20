#ifndef SQLITE3DB_H
#define SQLITE3DB_H

#include <v8.h>
extern "C"
{
#include <sqlite3.h>
}

#include "Sqlite3Query.h"
#include "Sqlite3Statement.h"
#include "Sqlite3Table.h"

#include "ObjectWrap.h"

namespace DCanvas
{

class SQLite3DB : public ObjectWrap
{
public:

    static SQLite3DB* createDB(){return new SQLite3DB();}
    static SQLite3DB* createDB(SQLite3DB& db){return new SQLite3DB(db);}

    virtual ~SQLite3DB();

    void open(const char* szFile);

    void close();

    bool tableExists(const char* szTable);

    int execDML(const char* szSQL);

    SQLite3Query* execQuery(const char* szSQL);

    int execScalar(const char* szSQL);

    SQLite3Table* getTable(const char* szSQL);

    SQLite3Statement* compileStatement(const char* szSQL);

    sqlite_int64 lastRowId();

    void interrupt() { sqlite3_interrupt(m_pDB); }

    void setBusyTimeout(int nMillisecs);

    static const char* SQLiteVersion() { return SQLITE_VERSION; }

    static v8::Handle<v8::Value> ExecDML(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> SQLiteVersion(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> CompileStatement(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> GetTable(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> ExecQuery(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> TableExists(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> ExecScalar(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Handle<v8::Value> LastRowId(const v8::FunctionCallbackInfo<v8::Value>& args);

    v8::Handle<v8::Value> wrap();

private:
    SQLite3DB();
    SQLite3DB(const SQLite3DB& db);
    SQLite3DB& operator=(const SQLite3DB& db);

    sqlite3_stmt* compile(const char* szSQL);

    void checkDB();

    sqlite3* m_pDB;
    int m_nBusyTimeoutMs;
};

}// namespace DCanvas
#endif //SQLITE3DB_H
