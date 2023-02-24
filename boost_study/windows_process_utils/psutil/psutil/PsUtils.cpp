#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <stdio.h>
#include <string.h>

#include <string>
#include <filesystem>
#include <fstream>

//#include <iostream>
//using std::cout;
//using std::endl;

bool ProcessExistByPidFile(const std::string& pidFilePath, std::string& err) {
	if (!std::filesystem::is_regular_file(pidFilePath)) {
		err = "file not exist. pidFile:" + pidFilePath;
		return false;
	}

	std::ifstream ifs(pidFilePath);

	DWORD pid = 0;

	ifs >> pid;
	if (pid == 0) {
		err = "pid read error. pidFile:" + pidFilePath;
		return false;
	}

	HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
	if (processHandle == NULL) {
		err = "OpenProcess failed. pidFile:" + pidFilePath;
		return false;
	}

	CloseHandle(processHandle);

	return true;
}

bool ProcessExistByPathKey(const std::string& processKey, std::string& err) {
	if (processKey.empty()) {
		err = "param error. processKey:" + processKey;
		return false;
	}

	bool ret = false;
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);

	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		err = "CreateToolhelp32Snapshot Error!";
		return false;
	}

	BOOL bResult = Process32First(hProcessSnap, &pe32);

	while (bResult) {
		std::string name = pe32.szExeFile;
		int pid = pe32.th32ProcessID;

		HANDLE processHandle = NULL;
		CHAR filename[MAX_PATH] = { 0 };
		processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
		if (processHandle != NULL) {
			if (GetModuleFileNameEx(processHandle, NULL, filename, MAX_PATH) == 0) {
				//cout << "Failed to get module filename." << endl;
			}
			else {
				//cout << "Module filename is: " << filename << endl;

				if (std::strstr(filename, processKey.c_str()) != nullptr) {
					ret = true;
				}
			}
			CloseHandle(processHandle);

			if (ret) break;
		}
		else {
			//cout << "Failed to open process." << endl;
		}

		//cout << "Process Name:" << name << "  " << "ProcessID:" << pid << endl;

		bResult = Process32Next(hProcessSnap, &pe32);
	}

	CloseHandle(hProcessSnap);

	return ret;
}

int CreateDetachProcess(const char* exe_path, const char* cmd_line_args, const char* workingDir) {
	PROCESS_INFORMATION p_info = { 0 };
	STARTUPINFO s_info = { 0 };

	s_info.cb = sizeof(STARTUPINFO);
	//s_info.dwFlags |= CREATE_NEW_CONSOLE;
	s_info.dwFlags |= DETACHED_PROCESS;

	if (!CreateProcess(exe_path, (LPSTR)cmd_line_args, NULL, NULL, FALSE,
		//CREATE_NEW_CONSOLE,
		//NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP,
		DETACHED_PROCESS,
		NULL, workingDir, &s_info, &p_info)) {
		return -1;
	}

	if (p_info.hProcess != 0) CloseHandle(p_info.hProcess);
	if (p_info.hThread != 0) CloseHandle(p_info.hThread);

	return 0;
}
