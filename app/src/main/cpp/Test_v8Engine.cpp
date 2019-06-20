#include "v8.h"
#include <string.h>
#include <stdio.h>

using namespace v8;
using namespace std;


Handle<Value> Yell(const v8::FunctionCallbackInfo<v8::Value>& args) {
	HandleScope  handle_scope;
	char buffer[4096];

	memset(buffer, 0, sizeof(buffer));
	Handle<String> str = args[0]->ToString();
	str->WriteUtf8(buffer);
	printf("Yell: %s\n", buffer);

	return Undefined();
}

int main(int argc, char** argv) {
	HandleScope handle_scope;

	//A
	Handle<FunctionTemplate> fun = FunctionTemplate::New(Yell);

	//B
	Handle<ObjectTemplate> global = ObjectTemplate::New();
	global->Set(String::New("yell"), fun);

	//C
	Persistent<Context> cxt = Context::New(NULL, global);

	Context::Scope context_scope(cxt);
	Handle<String> source = String::New("yell('Google V8!')");
	Handle<Script> script = Script::Compile(source);
	Handle<Value> result = script->Run();

	cxt.Dispose();
}
