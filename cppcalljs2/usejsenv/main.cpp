#include <node.h>
#include <v8.h>
#include <iostream>

using namespace v8;
using namespace std;

void GetArgument(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();

	// arguments length judgement
	if (args.Length() < 2) {
		isolate->ThrowException(Exception::TypeError(
			String::NewFromUtf8(isolate, "Wrong number of arguments")));
		return;
	}

	// arguments type judgement
	if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
		//throw exception
		isolate->ThrowException(Exception::TypeError(
			String::NewFromUtf8(isolate, "argumnets must be number")));
	}

	if (!args[0]->IsObject()) {
		printf("I am not Object\n");
	}

	if (!args[0]->IsBoolean()) {
		printf("I am not Boolean\n");
	}

	if (!args[0]->IsArray()) {
		printf("I am not Array\n");
	}

	if (!args[0]->IsString()) {
		printf("I am not String\n");
	}

	if (!args[0]->IsFunction()) {
		printf("I am not Function\n");
	}

	if (!args[0]->IsNull()) {
		printf("I am not Null\n");
	}

	if (!args[0]->IsUndefined()) {
		printf("I am not Undefined\n");
	}

	// js Number type convert to v8 Number type
	Local<Number> value1 = Local<Number>::Cast(args[0]);
	Local<Number> value2 = Local<Number>::Cast(args[1]);
	double value = value1->NumberValue() + value2->NumberValue();

	// js String type convert to v8 String type
	Local<String> str = Local<String>::Cast(args[2]);
	String::Utf8Value utfValue(str);
	cout << string(*utfValue) << endl;

	// js Array type convert to v8 Array type
	Local<Array> input_array = Local<Array>::Cast(args[3]);
	printf("%d, %f %f\n", input_array->Length(), input_array->Get(0)->NumberValue(), input_array->Get(1)->NumberValue());

	// js Object type convert to v8 Object type
	Local<Object> obj = Local<Object>::Cast(args[4]);

	// Get Object value from key
	Local<Value> a = obj->Get(String::NewFromUtf8(isolate, "a"));
	Local<Value> b = obj->Get(String::NewFromUtf8(isolate, "b"));

	// js Array type convert to v8 Array type
	Local<Array> c = Local<Array>::Cast(obj->Get(String::NewFromUtf8(isolate, "c")));
	cout << a->NumberValue() << "   " << b->NumberValue() << endl;
	printf("%d, %f %f\n", c->Length(), c->Get(0)->NumberValue(), c->Get(1)->NumberValue());

	// js String type convert to v8 String type
	Local<String> cString = Local<String>::Cast(c->Get(2));
	String::Utf8Value utfValueD(cString);
	cout << string(*utfValueD) << endl;

	// Get Object value from key
	Local<Object> d = Local<Object>::Cast(obj->Get(String::NewFromUtf8(isolate, "d")));
	Local<String> dString1 = Local<String>::Cast(d->Get(String::NewFromUtf8(isolate, "m")));
	String::Utf8Value utfValued1(dString1);
	cout << string(*utfValued1) << endl;

	// Get Object value from key
	Local<String> dString2 = Local<String>::Cast(d->Get(String::NewFromUtf8(isolate, "n")));
	String::Utf8Value utfValued2(dString2);
	cout << string(*utfValued2) << endl;

	// js Booelan type convert to v8 Boolean type
	Local<Boolean> FlagTrue = Local<Boolean>::Cast(args[5]);
	cout << "Flag: " << FlagTrue->BooleanValue() << endl;

	// js Function type convert to v8 Function type
	Local<Function> cb = Local<Function>::Cast(args[8]);
	const unsigned argc = 2;
	Local<Value> argv[2];
	argv[0] = a;
	argv[1] = b;
	cb->Call(Null(isolate), argc, argv);

	args.GetReturnValue().Set(value);
}

void Init(Local <Object> exports, Local <Object> module) {
	NODE_SET_METHOD(module, "exports", GetArgument);
}
NODE_MODULE(argumentss, Init)
