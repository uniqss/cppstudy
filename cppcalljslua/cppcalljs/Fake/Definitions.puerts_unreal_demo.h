#pragma once

#include <memory>

#if 0
// DLL export and import definitions
#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)
#else
#define DLLEXPORT 
#define DLLIMPORT 
#endif

#define JSENV_API DLLIMPORT

#define PUERTS_UNREAL_DEMO_API DLLEXPORT
#define PUERTS_API DLLIMPORT

#define GENERATED_USTRUCT_BODY(...) 
#define GENERATED_BODY(...) 

#define TSharedPtr std::shared_ptr
