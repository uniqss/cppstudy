#pragma once

#include "Definitions.puerts_unreal_demo.h"

#include <string>
#include <vector>

#define UPROPERTY(...)
#define UFUNCTION(...)
#define USTRUCT(...)
#define UMETA(...)
#define UPARAM(...)
#define UENUM(...)
#define UDELEGATE(...)
#define RIGVM_METHOD(...)
#define UCLASS(...)

#define FString std::string
#define TArray std::vector
#define TPair std::pair
#define UObject void
#define FName std::string

typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
#ifdef _MSC_VER
typedef unsigned __int64   uint64;
#else
typedef unsigned long long uint64;
#endif /* _MSC_VER */

class UBlueprintFunctionLibrary
{

};


//// If we don't have a platform-specific define for the TEXT macro, define it now.
//#if !defined(TEXT) && !UE_BUILD_DOCS
//#if PLATFORM_TCHAR_IS_CHAR16
//#define TEXT_PASTE(x) u ## x
//#else
//#define TEXT_PASTE(x) L ## x
//#endif
//#define TEXT(x) TEXT_PASTE(x)
//#endif

#define TEXT std::string

#define MakeShared std::make_shared
