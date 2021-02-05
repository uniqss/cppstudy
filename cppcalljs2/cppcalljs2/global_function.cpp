#include "global_function.h"





void AddGlobalFunction(v8::Local<v8::Context> context, v8::Isolate* isolate, const char* name, v8::FunctionCallback callback) {
	v8::Local<v8::FunctionTemplate> func_template =
		v8::FunctionTemplate::New(isolate, callback);
	v8::Local<v8::Function> func =
		func_template->GetFunction(context).ToLocalChecked();
	func->SetName(v8_str(name));
	context->Global()->Set(context, v8_str(name), func).FromJust();
}
