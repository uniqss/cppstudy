#pragma once

#include "stl.h"
#include "def.h"

class IJSModuleLoader
{
public:
	virtual bool Search(const std::string& RequiredDir, const std::string& RequiredModule, std::string& Path, std::string& AbsolutePath) = 0;

	virtual bool Load(const std::string& Path, TArray<uint8>& Content) = 0;

	virtual std::string& GetScriptRoot() = 0;

	virtual ~IJSModuleLoader() {}
};

class JSENV_API DefaultJSModuleLoader : public IJSModuleLoader
{
public:
	explicit DefaultJSModuleLoader(const std::string& InScriptRoot) : ScriptRoot(InScriptRoot) {}

	bool Search(const std::string& RequiredDir, const std::string& RequiredModule, std::string& Path, std::string& AbsolutePath) override;

	bool Load(const std::string& Path, TArray<uint8>& Content) override;

	std::string& GetScriptRoot() override;

private:
	bool CheckExists(const FString& PathIn, FString& Path, FString& AbsolutePath);

	bool SearchModuleInDir(const FString& Dir, const FString& RequiredModule, FString& Path, FString& AbsolutePath);

	bool SearchModuleWithExtInDir(const FString& Dir, const FString& RequiredModule, FString& Path, FString& AbsolutePath);

	FString ScriptRoot;
};

