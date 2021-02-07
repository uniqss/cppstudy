#include "v8_inc.h"

#include "test_call_v8.h"

int main()
{
	std::vector<std::string> scripts;
	std::string lib_path;

	//v8::V8::InitializeICU();
	v8::V8::InitializeExternalStartupData("./");
	std::unique_ptr<v8::Platform> platform(v8::platform::NewDefaultPlatform());
	v8::V8::InitializePlatform(platform.get());
	v8::V8::Initialize();

	test_call_v8();


	v8::V8::Dispose();
	v8::V8::ShutdownPlatform();

	return 0;
}
