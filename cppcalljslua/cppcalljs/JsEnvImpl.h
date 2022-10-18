#pragma once

#include "JsEnv.h"

#include "V8Utils.h"

#include "JSLogger.h"

#pragma warning(push, 0)  
#include "libplatform/libplatform.h"
#include "v8.h"
#pragma warning(pop)

#include "V8InspectorImpl.h"

namespace puerts
{

	class FJsEnvImpl : public IJsEnv
	{
		explicit FJsEnvImpl(const FString& ScriptRoot);

		FJsEnvImpl(std::unique_ptr<IJSModuleLoader> InModuleLoader, std::shared_ptr<ILogger> InLogger, int Port);

		~FJsEnvImpl() override;

		void Start(const FString& ModuleName, const TArray<TPair<FString, UObject*>>& Arguments) override;

		void LowMemoryNotification() override;

		void WaitDebugger() override
		{
			while (Inspector && !Inspector->Tick()) {}
		}

		virtual void TryBindJs(const class UObjectBase* InObject) override;

		virtual void RebindJs() override;

		void ReloadJsModule(FName ModuleName);

		virtual void ReloadModule(FName ModuleName) override;

	public:

		V8Inspector* Inspector;
	};
}
