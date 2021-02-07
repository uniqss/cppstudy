#include "v8_inc.h"

#include "v8pp.h"

#include "test_call_v8.h"

#include "common_functions.h"

int main()
{
	std::vector<std::string> scripts;
	std::string lib_path;

	//v8::V8::InitializeICU();
	v8::V8::InitializeExternalStartupData("./");
	std::unique_ptr<v8::Platform> platform(v8::platform::NewDefaultPlatform());
	v8::V8::InitializePlatform(platform.get());
	v8::V8::Initialize();


	v8pp::context context;


	set_common_functions(context);
	test_call_v8(context);


	v8::V8::Dispose();
	v8::V8::ShutdownPlatform();

	return 0;
}
