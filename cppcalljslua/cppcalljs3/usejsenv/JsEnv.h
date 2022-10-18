#pragma once

#include "stl.h"
#include "def.h"
#include "JsModuleLoader.h"
#include "JsLogger.h"

class JSENV_API IJsEnv
{
public:
	virtual void Start(const std::string& ModuleName, const std::vector<std::pair<std::string, void*>>& Arguments) = 0;

	virtual void LowMemoryNotification() = 0;

	virtual void WaitDebugger() = 0;

	virtual void TryBindJs(const class UObjectBase* InObject) = 0;

	virtual void ReloadModule(std::string ModuleName) = 0;

	virtual void RebindJs() = 0;

	virtual ~IJsEnv() {}
};

class JSENV_API FJsEnv// : public TSharedFromThis<FJsEnv> // only a wrapper
{
public:
	explicit FJsEnv(const std::string& ScriptRoot = ("JavaScript"));

	FJsEnv(std::unique_ptr<IJSModuleLoader> InModuleLoader, std::shared_ptr<ILogger> InLogger, int InDebugPort);

	void Start(const std::string& ModuleName, const std::vector<std::pair<std::string, void*>>& Arguments = std::vector<std::pair<std::string, void*>>());

	void LowMemoryNotification();

	void WaitDebugger();

	void TryBindJs(const class UObjectBase* InObject);

	//ModuleName等于NAME_None代表从新加载所有脚本
	void ReloadModule(std::string ModuleName);

	void RebindJs();

private:
	std::unique_ptr<IJsEnv> GameScript;
};
