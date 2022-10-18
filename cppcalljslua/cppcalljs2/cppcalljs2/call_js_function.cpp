#include <include/v8.h>

#include <include/libplatform/libplatform.h>

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>

#include "global_function.h"

using namespace v8;

const char* ToCString(const v8::String::Utf8Value& value) {
	return *value ? *value : "<string conversion failed>";
}

void Print(const v8::FunctionCallbackInfo<v8::Value>& args) {
	bool first = true;
	for (int i = 0; i < args.Length(); i++) {
		v8::HandleScope handle_scope(args.GetIsolate());
		if (first) {
			first = false;
		}
		else {
			printf(" ");
		}
		v8::String::Utf8Value str(args.GetIsolate(), args[i]);
		const char* cstr = ToCString(str);
		printf("%s", cstr);
	}
	printf("\n");
	fflush(stdout);
}

v8::Local<v8::Context> CreateShellContext(v8::Isolate* isolate) {
	// Create a template for the global object.
	v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
	// Bind the global 'print' function to the C++ Print callback.
	global->Set(isolate, "print", v8::FunctionTemplate::New(isolate, Print));
	//// Bind the global 'read' function to the C++ Read callback.
	//global->Set(isolate, "read", v8::FunctionTemplate::New(isolate, Read));
	//// Bind the global 'load' function to the C++ Load callback.
	//global->Set(isolate, "load", v8::FunctionTemplate::New(isolate, Load));
	//// Bind the 'quit' function
	//global->Set(isolate, "quit", v8::FunctionTemplate::New(isolate, Quit));
	//// Bind the 'version' function
	//global->Set(isolate, "version", v8::FunctionTemplate::New(isolate, Version));
	return v8::Context::New(isolate, NULL, global);
}

void UniqsLog(const v8::FunctionCallbackInfo<v8::Value>& args) {
	bool first = true;
	for (int i = 0; i < args.Length(); i++) {
		v8::HandleScope handle_scope(args.GetIsolate());
		if (first) {
			first = false;
		}
		else {
			printf(" ");
		}
		v8::String::Utf8Value str(args.GetIsolate(), args[i]);
		const char* cstr = ToCString(str);
		printf("%s", cstr);
	}
	printf("\n");
	fflush(stdout);
}

int main(int argc, char* argv[]) {


	v8::V8::InitializeICUDefaultLocation(argv[0]);
	v8::V8::InitializeExternalStartupData(argv[0]);
	std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
	v8::V8::InitializePlatform(platform.get());
	v8::V8::Initialize();
	v8::V8::SetFlagsFromCommandLine(&argc, argv, true);
	v8::Isolate::CreateParams create_params;
	create_params.array_buffer_allocator =
		v8::ArrayBuffer::Allocator::NewDefaultAllocator();
	v8::Isolate* isolate = v8::Isolate::New(create_params);

	v8::Isolate::Scope isolate_scope(isolate);
	v8::HandleScope handle_scope(isolate);
	v8::Local<v8::Context> context = CreateShellContext(isolate);
	if (context.IsEmpty()) {
		fprintf(stderr, "Error creating context\n");
		return 1;
	}
	v8::Context::Scope context_scope(context);

	Handle<v8::Object> global = context->Global();

	AddGlobalFunction(context, isolate, "__uniqs_log", UniqsLog);

	v8::Local<v8::String> name = v8::String::NewFromUtf8(
		isolate, "myJsFunction", v8::NewStringType::kInternalized).ToLocalChecked();
	v8::Local<v8::Value> obj =
		context->Global()->Get(context, name).ToLocalChecked();
	if (!obj->IsFunction()) {
		/* someone overwrote it, handle error */
		fprintf(stderr, "someone overwrote it. error occurred!");
		return 2;
	}
	v8::Local<v8::Function> my_function = v8::Local<v8::Function>::Cast(obj);

	v8::Local<v8::Value> receiver;
#define MYJSFUNCTION_argc 2

	v8::Local<v8::Value> p1 = v8::Uint32::NewFromUnsigned(isolate, 1234);
	v8::Local<v8::Value> p2 = v8::String::NewFromUtf8(isolate, "adsf").ToLocalChecked();
	v8::Local<v8::Value> args[MYJSFUNCTION_argc] = { p1, p2 };
	v8::MaybeLocal<v8::Value> result = my_function->Call(context, receiver, argc, args);
	v8::Local<v8::Value> result_local = result.ToLocalChecked();

	uint32_t retJust = result_local->Uint32Value(context).FromJust();
	uint32_t retMaybe = result_local->Uint32Value(context).FromMaybe(0);
	bool retIsValue = result_local->IsValue();
	printf("retIsValue:%d retJust:%d retMaybe:%d", (int)retIsValue, retJust, retMaybe);


	isolate->Dispose();
	v8::V8::Dispose();
	v8::V8::ShutdownPlatform();

	return 0;

}