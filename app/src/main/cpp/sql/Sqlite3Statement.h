#ifndef SQLITE3STATEMENT_H
#define SQLITE3STATEMENT_H

#include "Sqlite3Query.h"
extern "C"
{
#include <sqlite3.h>
}
#include <v8.h>
#include "ObjectWrap.h"

namespace DCanvas
{

class SQLite3Statement : public ObjectWrap
{
public:

    SQLite3Statement();

    SQLite3Statement(const SQLite3Statement& rStatement);

    SQLite3Statement(sqlite3* pDB, sqlite3_stmt* pVM);

    virtual ~SQLite3Statement();

    SQLite3Statement& operator=(const SQLite3Statement& rStatement);

    int execDML();

    SQLite3Query* execQuery();

    void bind(int nParam, const char* szValue);
    void bind(int nParam, const int nValue);
    void bind(int nParam, const double dwValue);
    void bind(int nParam, const unsigned char* blobValue, int nLen);
    void bindNull(int nParam);

    void reset();

    int finalize();

	v8::Handle<v8::Value> wrap();
	
	static v8::Handle<v8::Value> ExecDML(const v8::FunctionCallbackInfo<v8::Value>& args);

	static v8::Handle<v8::Value> ExecQuery(const v8::FunctionCallbackInfo<v8::Value>& args);
	
	static v8::Handle<v8::Value> Bind(const v8::FunctionCallbackInfo<v8::Value>& args);
	
	static v8::Handle<v8::Value> BindNull(const v8::FunctionCallbackInfo<v8::Value>& args);
	
	static v8::Handle<v8::Value> Reset(const v8::FunctionCallbackInfo<v8::Value>& args);
	
	static v8::Handle<v8::Value> Finalize(const v8::FunctionCallbackInfo<v8::Value>& args);
	
private:

    void checkDB();
    void checkVM();

    sqlite3* m_pDB;
    sqlite3_stmt* m_pVM;
};

} // namespace DCanvas

#endif //SQLITE3STATEMENT_H
