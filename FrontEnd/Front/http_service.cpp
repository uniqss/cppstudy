#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "http_service.h"

#include "asyncio.h"

#include "json.hpp"

#include <locale>
#include <codecvt>
#include <string>

#include <thread>

void HttpThread(std::function<void()> exitCb) {
	asyncio::SetLogHandler(
		[](asyncio::Log::LogLevel lv, const char* msg) {
			std::string time_now = asyncio::util::Time::FormatDateTime(std::chrono::system_clock::now());
			switch (lv) {
			case asyncio::Log::kError:
				printf("%s Error: %s\n", time_now.c_str(), msg);
				break;
			case asyncio::Log::kWarning:
				printf("%s Warning: %s\n", time_now.c_str(), msg);
				break;
			case asyncio::Log::kInfo:
				printf("%s Info: %s\n", time_now.c_str(), msg);
				break;
			case asyncio::Log::kDebug:
				printf("%s Debug: %s\n", time_now.c_str(), msg);
				break;
			default:
				break;
			}
		},
		asyncio::Log::kDebug);

	int port = 65533;
	asyncio::EventLoop my_event_loop;
	auto http_server = my_event_loop.CreateHttpServer(port, [](asyncio::http::connection_ptr conn) {
		const auto& cmd = conn->get_req().action;
		const auto& body = conn->get_req().body;
		ASYNCIO_LOG_DEBUG("GmCmd:%s, Body:%s", cmd.data(), body.data());
		bool ok = false;
		std::string err_msg;
		do
		{
			// exe_path
			nlohmann::json bodyJson = nlohmann::json::parse(body, nullptr, false, true);
			if (!bodyJson.contains("exe_path")) {
				err_msg = "exe_path invalid";
				break;
			}
			std::string exe_path = bodyJson["exe_path"];

			// cmd_line_args
			if (!bodyJson.contains("cmd_line_args")) {
				err_msg = "cmd_line_args invalid";
				break;
			}
			std::string cmd_line_args = bodyJson["cmd_line_args"];

			// workingDir
			if (!bodyJson.contains("workingDir")) {
				err_msg = "workingDir invalid";
				break;
			}
			std::string workingDir = bodyJson["workingDir"];

			//// workingDirs
			//if (!bodyJson.contains("workingDirs")) {
			//	err_msg = "workingDirs invalid";
			//	break;
			//}
			//std::string workingDirs = bodyJson["workingDirs"];

			CreateDetachProcess(exe_path.c_str(), cmd_line_args.c_str(), workingDir.c_str());

			ok = true;

		} while (false);

		std::stringstream ss;
		ss << "{\"ret:";
		if (ok) {
			ss << "\"ok\"";
		}
		else {
			ss << "\"failed\"";
		}
		ss << "}";

		conn->get_rep().content = "{\"ret:\" +  + "}";
		conn->do_write();
});

	my_event_loop.RunForever();

	exitCb();
}


bool StartHttpService(std::function<void()> exitCb)
{
	std::thread t(HttpThread, exitCb);
	t.detach();

	return 0;
}


#ifdef _WIN32
int to_wchar(const char* path, wchar_t* wbuf, size_t wbuf_len) {
	int ret;
	char buf[MAX_PATH * 2], buf2[MAX_PATH * 2], * p;

	strncpy_s(buf, path, sizeof(buf));
	buf[sizeof(buf) - 1] = '\0';

	/* Trim trailing slashes. Leave backslash for paths like "X:\" */
	p = buf + strlen(buf) - 1;
	while (p > buf && p[-1] != ':' && (p[0] == '\\' || p[0] == '/')) *p-- = '\0';

	memset(wbuf, 0, wbuf_len * sizeof(wchar_t));
	ret = MultiByteToWideChar(CP_UTF8, 0, buf, -1, wbuf, (int)wbuf_len);

	/*
	 * Convert back to Unicode. If doubly-converted string does not match the
	 * original, something is fishy, reject.
	 */
	WideCharToMultiByte(CP_UTF8, 0, wbuf, (int)wbuf_len, buf2, sizeof(buf2),
		NULL, NULL);
	if (strcmp(buf, buf2) != 0) {
		wbuf[0] = L'\0';
		ret = 0;
	}

	return ret;
}
#endif /* _WIN32 */

int CreateDetachProcess(const std::string& exe_path, const std::string& cmd_line_args, const std::string& workingDir) {
#if 0

	std::string cmd = exe_path + " " + cmd_line_args;
	//cout << "cmd:" << cmd << endl;

	int ret = std::system(cmd.c_str());
#endif

#if 1
	PROCESS_INFORMATION p_info = { 0 };
	STARTUPINFO s_info = { 0 };

	s_info.cb = sizeof(STARTUPINFO);
	//s_info.dwFlags |= CREATE_NEW_CONSOLE;
	s_info.dwFlags |= DETACHED_PROCESS;

	if (!CreateProcess(exe_path.c_str(), (LPSTR)cmd_line_args.c_str(), NULL, NULL, FALSE,
		//CREATE_NEW_CONSOLE,
		//NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP,
		DETACHED_PROCESS,
		NULL, workingDir.c_str(), &s_info, &p_info)) {
		return -1;
	}

	if (p_info.hProcess != 0) CloseHandle(p_info.hProcess);
	if (p_info.hThread != 0) CloseHandle(p_info.hThread);

#endif

	return 0;
}
