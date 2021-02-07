#pragma once
#include "v8_inc.h"


enum UniqsV8Types {
	UniqsV8Types_Undefined,
	UniqsV8Types_Null,
	UniqsV8Types_True,
	UniqsV8Types_False,
	UniqsV8Types_Name,
	UniqsV8Types_String,
	UniqsV8Types_Boolean,
	UniqsV8Types_Number,
	UniqsV8Types_External,
	UniqsV8Types_Int32,
	UniqsV8Types_Uint32,
	UniqsV8Types_Date,
	UniqsV8Types_ArgumentsObject,
	UniqsV8Types_BooleanObject,
	UniqsV8Types_NumberObject,
	UniqsV8Types_StringObject,
	UniqsV8Types_SymbolObject,
	UniqsV8Types_NativeError,
	UniqsV8Types_RegExp,
	UniqsV8Types_GeneratorFunction,
	UniqsV8Types_GeneratorObject,
	UniqsV8Types_Promise,
	UniqsV8Types_Map,
	UniqsV8Types_Set,
	UniqsV8Types_MapIterator,
	UniqsV8Types_SetIterator,
	UniqsV8Types_WeakMap,
	UniqsV8Types_WeakSet,
	UniqsV8Types_ArrayBuffer,
	UniqsV8Types_ArrayBufferView,
	UniqsV8Types_TypedArray,
	UniqsV8Types_Uint8Array,
	UniqsV8Types_Uint8ClampedArray,
	UniqsV8Types_Int8Array,
	UniqsV8Types_Uint16Array,
	UniqsV8Types_Int16Array,
	UniqsV8Types_Uint32Array,
	UniqsV8Types_Int32Array,
	UniqsV8Types_Float32Array,
	UniqsV8Types_Float64Array,
	UniqsV8Types_DataView,
	UniqsV8Types_SharedArrayBuffer,
	UniqsV8Types_Symbol,
	UniqsV8Types_Function,
	UniqsV8Types_Array,
	UniqsV8Types_Object,
};

static const char* UniqsV8TypesString[] = {
	"Undefined",
	"Null",
	"True",
	"False",
	"Name",
	"String",
	"Boolean",
	"Number",
	"External",
	"Int32",
	"Uint32",
	"Date",
	"ArgumentsObject",
	"BooleanObject",
	"NumberObject",
	"StringObject",
	"SymbolObject",
	"NativeError",
	"RegExp",
	"GeneratorFunction",
	"GeneratorObject",
	"Promise",
	"Map",
	"Set",
	"MapIterator",
	"SetIterator",
	"WeakMap",
	"WeakSet",
	"ArrayBuffer",
	"ArrayBufferView",
	"TypedArray",
	"Uint8Array",
	"Uint8ClampedArray",
	"Int8Array",
	"Uint16Array",
	"Int16Array",
	"Uint32Array",
	"Int32Array",
	"Float32Array",
	"Float64Array",
	"DataView",
	"SharedArrayBuffer",
	"Symbol",
	"Function",
	"Array",
	"Object",
};

static const char* UniqsV8GetType(const v8::Local<v8::Value>& v8value)
{
	if (v8value->IsUndefined()) return UniqsV8TypesString[UniqsV8Types_Undefined];
	if (v8value->IsNull()) return UniqsV8TypesString[UniqsV8Types_Null];
	if (v8value->IsTrue()) return UniqsV8TypesString[UniqsV8Types_True];
	if (v8value->IsFalse()) return UniqsV8TypesString[UniqsV8Types_False];
	if (v8value->IsName()) return UniqsV8TypesString[UniqsV8Types_Name];
	if (v8value->IsString()) return UniqsV8TypesString[UniqsV8Types_String];
	if (v8value->IsBoolean()) return UniqsV8TypesString[UniqsV8Types_Boolean];
	if (v8value->IsNumber()) return UniqsV8TypesString[UniqsV8Types_Number];
	if (v8value->IsExternal()) return UniqsV8TypesString[UniqsV8Types_External];
	if (v8value->IsInt32()) return UniqsV8TypesString[UniqsV8Types_Int32];
	if (v8value->IsUint32()) return UniqsV8TypesString[UniqsV8Types_Uint32];
	if (v8value->IsDate()) return UniqsV8TypesString[UniqsV8Types_Date];
	if (v8value->IsArgumentsObject()) return UniqsV8TypesString[UniqsV8Types_ArgumentsObject];
	if (v8value->IsBooleanObject()) return UniqsV8TypesString[UniqsV8Types_BooleanObject];
	if (v8value->IsNumberObject()) return UniqsV8TypesString[UniqsV8Types_NumberObject];
	if (v8value->IsStringObject()) return UniqsV8TypesString[UniqsV8Types_StringObject];
	if (v8value->IsSymbolObject()) return UniqsV8TypesString[UniqsV8Types_SymbolObject];
	if (v8value->IsNativeError()) return UniqsV8TypesString[UniqsV8Types_NativeError];
	if (v8value->IsRegExp()) return UniqsV8TypesString[UniqsV8Types_RegExp];
	if (v8value->IsGeneratorFunction()) return UniqsV8TypesString[UniqsV8Types_GeneratorFunction];
	if (v8value->IsGeneratorObject()) return UniqsV8TypesString[UniqsV8Types_GeneratorObject];
	if (v8value->IsPromise()) return UniqsV8TypesString[UniqsV8Types_Promise];
	if (v8value->IsMap()) return UniqsV8TypesString[UniqsV8Types_Map];
	if (v8value->IsSet()) return UniqsV8TypesString[UniqsV8Types_Set];
	if (v8value->IsMapIterator()) return UniqsV8TypesString[UniqsV8Types_MapIterator];
	if (v8value->IsSetIterator()) return UniqsV8TypesString[UniqsV8Types_SetIterator];
	if (v8value->IsWeakMap()) return UniqsV8TypesString[UniqsV8Types_WeakMap];
	if (v8value->IsWeakSet()) return UniqsV8TypesString[UniqsV8Types_WeakSet];
	if (v8value->IsArrayBuffer()) return UniqsV8TypesString[UniqsV8Types_ArrayBuffer];
	if (v8value->IsArrayBufferView()) return UniqsV8TypesString[UniqsV8Types_ArrayBufferView];
	if (v8value->IsTypedArray()) return UniqsV8TypesString[UniqsV8Types_TypedArray];
	if (v8value->IsUint8Array()) return UniqsV8TypesString[UniqsV8Types_Uint8Array];
	if (v8value->IsUint8ClampedArray()) return UniqsV8TypesString[UniqsV8Types_Uint8ClampedArray];
	if (v8value->IsInt8Array()) return UniqsV8TypesString[UniqsV8Types_Int8Array];
	if (v8value->IsUint16Array()) return UniqsV8TypesString[UniqsV8Types_Uint16Array];
	if (v8value->IsInt16Array()) return UniqsV8TypesString[UniqsV8Types_Int16Array];
	if (v8value->IsUint32Array()) return UniqsV8TypesString[UniqsV8Types_Uint32Array];
	if (v8value->IsInt32Array()) return UniqsV8TypesString[UniqsV8Types_Int32Array];
	if (v8value->IsFloat32Array()) return UniqsV8TypesString[UniqsV8Types_Float32Array];
	if (v8value->IsFloat64Array()) return UniqsV8TypesString[UniqsV8Types_Float64Array];
	if (v8value->IsDataView()) return UniqsV8TypesString[UniqsV8Types_DataView];
	if (v8value->IsSharedArrayBuffer()) return UniqsV8TypesString[UniqsV8Types_SharedArrayBuffer];
	if (v8value->IsSymbol()) return UniqsV8TypesString[UniqsV8Types_Symbol];
	if (v8value->IsFunction()) return UniqsV8TypesString[UniqsV8Types_Function];
	if (v8value->IsArray()) return UniqsV8TypesString[UniqsV8Types_Array];
	if (v8value->IsObject()) return UniqsV8TypesString[UniqsV8Types_Object];
	return(0);
}
