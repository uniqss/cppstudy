#include "test_call_v8.h"

#include "v8pp.h"
#include "test.hpp"

#include "uniqsv8gettype.h"

static void v8_arg_count(v8::FunctionCallbackInfo<v8::Value> const& args)
{
	args.GetReturnValue().Set(args.Length());
}

void test_call_v8()
{
	v8pp::context context;

	v8::Isolate* isolate = context.isolate();
	v8::HandleScope scope(isolate);

#if 0
	v8::Local<v8::Function> fun = v8::Function::New(isolate->GetCurrentContext(), v8_arg_count).ToLocalChecked();

	check_eq("no args",
		v8pp::call_v8(isolate, fun, fun)->Int32Value(isolate->GetCurrentContext()).FromJust(), 0);
	check_eq("1 arg",
		v8pp::call_v8(isolate, fun, fun, 1)->Int32Value(isolate->GetCurrentContext()).FromJust(), 1);
	check_eq("2 args",
		v8pp::call_v8(isolate, fun, fun, true, 2.2)->Int32Value(isolate->GetCurrentContext()).FromJust(), 2);
	check_eq("3 args",
		v8pp::call_v8(isolate, fun, fun, 1, true, "abc")->Int32Value(isolate->GetCurrentContext()).FromJust(), 3);

	v8::TryCatch try_catch(isolate);
#endif
	//fun = context.run_script("var fun = function() { non_existing_fun(); }; fun").As<v8::Function>();
	//check("empty result",
	//	v8pp::call_v8(isolate, fun, fun).IsEmpty());
	//check("has caught", try_catch.HasCaught());

#if 1
	v8::Local<v8::Function> fun = context.run_file("shelltest.js").As<v8::Function>();
	int funret = v8pp::call_v8(isolate, fun, fun, 1, 2)->Int32Value(isolate->GetCurrentContext()).FromJust();
	printf("funret:%d\n", funret);
#endif

#if 0
	v8::Local<v8::Array> funArr = context.run_file("shelltest.js").As<v8::Array>();
	std::array < v8::Local<v8::Function>, 2> funArrCpp = v8pp::from_v8< std::array<v8::Local<v8::Function>, 2>>(isolate, funArr);

	for (auto& it : funArrCpp)
	{
		int funret = v8pp::call_v8(isolate, it, it)->Int32Value(isolate->GetCurrentContext()).FromJust();
		printf("funret:%d\n", funret);
	}

	const char* type = UniqsV8GetType(funArr);
	printf("shelltest.js return UniqsV8GetType:%s\n", type);
#endif

}

