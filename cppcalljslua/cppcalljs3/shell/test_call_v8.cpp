#include "test_call_v8.h"

#include "v8pp.h"
#include "test.hpp"

#include "uniqsv8gettype.h"
#include <vector>


static void v8_arg_count(v8::FunctionCallbackInfo<v8::Value> const& args)
{
	args.GetReturnValue().Set(args.Length());
}

void test_call_v8(v8pp::context& context)
{
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

#if 0
	v8::Local<v8::Function> fun = context.run_file("shelltest.js").As<v8::Function>();
	int funret = v8pp::call_v8(isolate, fun, fun, 1, 2)->Int32Value(isolate->GetCurrentContext()).FromJust();
	printf("funret:%d\n", funret);
#endif

	// test call js return multi function
#if 0
	v8::Local<v8::Array> funs = context.run_file("shelltest_return_multi_function.js").As<v8::Array>();
	for (uint32_t i = 0, count = funs->Length();i < count;++i)
	{
		v8::Local<v8::Function> fun = funs->Get(isolate->GetCurrentContext(), i).ToLocalChecked().As<v8::Function>();
		int funret = v8pp::call_v8(isolate, fun, fun, 11, 22)->Int32Value(isolate->GetCurrentContext()).FromJust();
		printf("funret:%d\n", funret);
	}
#endif

	// pass multi param and pass array
#if 1
	v8::Local<v8::Function> fun = context.run_file("shelltest_calljsfunc_pass_array.js").As<v8::Function>();
	std::vector<std::string> strArr1 = { "asdf", "jjjj" };
	std::vector<std::string> strArr2 = { "qqqq", "wwww" };
	std::vector<int> intArr1 = { 111, 222, 333 };
	std::vector<int> intArr2 = { 5555, 222, 111 };

#if 0
	std::string funret = v8pp::from_v8<std::string>(isolate,
		v8pp::call_v8(isolate, fun, fun, "asdf", "jjj", 1234, 5678)->ToString(isolate->GetCurrentContext()).ToLocalChecked());
#else 
	std::string funret = v8pp::from_v8<std::string>(isolate,
		v8pp::call_v8(isolate, fun, fun, strArr1, strArr2, intArr1, intArr2)->ToString(isolate->GetCurrentContext()).ToLocalChecked());
#endif
	printf("funret:%s\n", funret.c_str());
#endif

}

void test_call_v8_post()
{

}

