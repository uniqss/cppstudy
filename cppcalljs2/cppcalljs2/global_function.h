#pragma once

#include "v8_inc.h"

static void AddGlobalFunction(v8::Local<v8::Context> context, v8::Isolate* isolate, const char* name, v8::FunctionCallback callback = nullptr);


static inline v8::Local<v8::String> v8_str(const char* x) {
	return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), x).ToLocalChecked();
}
