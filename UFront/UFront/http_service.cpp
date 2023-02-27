#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "http_service.h"

#ifndef ASIO_STANDALONE
#define ASIO_STANDALONE
#endif
#ifndef ASYNC_SIMPLE_HAS_NOT_AIO
#define ASYNC_SIMPLE_HAS_NOT_AIO
#endif
#include "../cinatra/include/cinatra.hpp"

#include "../../../yalantinglibs/include/easylog/easylog.h"

#include "json.hpp"

#include <locale>
#include <codecvt>
#include <string>
#include <thread>
#include <filesystem>

using namespace easylog;

int HttpThread(std::function<void()> exitCb) {
    std::filesystem::create_directories("./log/");
    std::string filename = "./log/front.log";
    easylog::init_log(Severity::DEBUG, filename, false, 5000, 10, true);

    cinatra::http_server server(std::thread::hardware_concurrency());
    bool r = server.listen("0.0.0.0", "65533");
    if (!r) {
        // LOG_INFO << "listen failed";
        return -1;
    }

    server.set_http_handler<cinatra::GET, cinatra::POST>(
        "/", [](cinatra::request& req, cinatra::response& res) mutable {
            std::string body(req.body());

            //ELOGV(INFO, "body:%s", body.c_str());
            ELOGD << "body:" << body;

            bool ok = false;
            std::string err_msg;
            do {
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
                // if (!bodyJson.contains("workingDirs")) {
                //	err_msg = "workingDirs invalid";
                //	break;
                // }
                // std::string workingDirs = bodyJson["workingDirs"];

                CreateDetachProcess(exe_path.c_str(), cmd_line_args.c_str(), workingDir.c_str());

                ok = true;

            } while (false);

            std::stringstream ss;
            ss << "{";
            ss << "\"ret:";
            if (ok) {
                ss << "\"ok\"";
            } else {
                ss << "\"failed\"";
                ss << ", ";
                ss << "\"err_msg\":";
                ss << "\"";
                ss << err_msg;
                ss << "\"";
            }
            ss << "}";

            res.set_status_and_content(cinatra::status_type::ok, ss.str());
            // res.set_status_and_content(status_type::ok, std::move(str));
        });

    asio::signal_set signals(server.get_io_service());

    signals.add(SIGINT);
    signals.add(SIGTERM);
#if defined(SIGQUIT)
    signals.add(SIGQUIT);
#endif
    signals.async_wait([&server](asio::error_code ec, int) {
        std::cout << "Stop server since receive signal to quit." << std::endl;
        server.stop();
    });

    server.run();

    exitCb();

    return 0;
}


bool StartHttpService(std::function<void()> exitCb) {
    std::thread t(HttpThread, exitCb);
    t.detach();

    return 0;
}


#ifdef _WIN32
int to_wchar(const char* path, wchar_t* wbuf, size_t wbuf_len) {
    int ret;
    char buf[MAX_PATH * 2], buf2[MAX_PATH * 2], *p;

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
    WideCharToMultiByte(CP_UTF8, 0, wbuf, (int)wbuf_len, buf2, sizeof(buf2), NULL, NULL);
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
    PROCESS_INFORMATION p_info = {0};
    STARTUPINFOA s_info = {0};

    s_info.cb = sizeof(STARTUPINFO);
    // s_info.dwFlags |= CREATE_NEW_CONSOLE;
    s_info.dwFlags |= DETACHED_PROCESS;

    if (!CreateProcessA(exe_path.c_str(), (LPSTR)cmd_line_args.c_str(), NULL, NULL, FALSE,
                        // CREATE_NEW_CONSOLE,
                        // NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP,
                        DETACHED_PROCESS, NULL, workingDir.c_str(), &s_info, &p_info)) {
        return -1;
    }

    if (p_info.hProcess != 0) CloseHandle(p_info.hProcess);
    if (p_info.hThread != 0) CloseHandle(p_info.hThread);

#endif

    return 0;
}
