#include "PsUtils.h"

#include <iostream>
using std::cout;
using std::endl;


int main() {
	std::string pidPath = "E:\\work\\ws2\\legendserver\\output\\Run\\eslib\\Debug\\pid\\loginserverD.pid";
	std::string err;
	bool ret = false;
	ret = ProcessExistByPidFile(pidPath, err);
	cout << "ret:" << ret << " err:" << err << endl;

	std::string pidKey = "E:\\work\\ws2\\legendserver\\output\\Run\\eslib\\Debug\\loginserverD.exe";
	ret = ProcessExistByPathKey(pidKey, err);
	cout << "ret:" << ret << " err:" << err << endl;

	int nret = 0;
	//std::string exePath = "E:\\work\\ws2\\legendserver\\output\\Run\\eslib\\Debug\\loginserverD.exe";
	//std::string workingDir = "E:\\work\\ws2\\legendserver\\output\\Run\\eslib\\Debug\\";
	//nret = CreateDetachProcess(exePath.c_str(), "", workingDir.c_str());
	//nret = CreateDetachProcess("C:\\Windows\\System32\\cmd.exe", " E:\\work\\ws2\\legendserver\\output\\Run\\eslib\\Debug\\loginserverD.exe", workingDir.c_str());
	//nret = CreateDetachProcess("C:\\Windows\\System32\\cmd.exe E:\\work\\ws2\\legendserver\\output\\Run\\1_StartAllDebug.bat", " ", workingDir.c_str());
	//nret = CreateDetachProcess("C:\\Windows\\System32\\cmd.exe", "/c E:\\work\\ws2\\legendserver\\output\\Run\\1_StartAllDebug.bat", "E:\\work\\ws2\\legendserver\\output\\Run\\");
	//nret = CreateDetachProcess("C:\\Windows\\System32\\cmd.exe", "/c E:\\work\\ws3\\legendserver\\output\\Run\\1_StartAllDebug.bat", "E:\\work\\ws3\\legendserver\\output\\Run\\");
	//nret = CreateDetachProcess("C:\\Windows\\System32\\cmd.exe", "/c E:\\work\\ws2\\legendserver\\output\\Run\\1_StartAllDebug.bat", "E:\\work\\ws2\\legendserver\\output\\Run\\");
	//nret = CreateDetachProcess("C:\\Windows\\System32\\cmd.exe", "/c E:\\work\\ws3\\legendserver\\output\\Run\\1_StopAllDebug.bat", "E:\\work\\ws3\\legendserver\\output\\Run\\");
	//nret = CreateDetachProcess("C:\\Windows\\System32\\cmd.exe", "/c E:\\work\\ws2\\legendserver\\output\\Run\\1_StopAllDebug.bat", "E:\\work\\ws2\\legendserver\\output\\Run\\");
	//nret = CreateDetachProcess("C:\\Windows\\System32\\cmd.exe", "/c E:\\work\\ds1\\legendserver\\branches\\branch_agent\\output\\Run\\Release\\2_StopAllRelease.bat", "E:\\work\\ds1\\legendserver\\branches\\branch_agent\\output\\Run\\Release\\");
	//nret = CreateDetachProcess("E:\\work\\ds1\\legendserver\\branches\\branch_agent\\output\\Run\\Release\\Release\\sync_call_system.exe", "/c E:\\work\\ds1\\legendserver\\branches\\branch_agent\\output\\Run\\Release\\2_StopAllRelease.bat", "E:\\work\\ds1\\legendserver\\branches\\branch_agent\\output\\Run\\Release\\");
	//nret = CreateDetachProcess("E:\\work\\ds1\\legendserver\\branches\\branch_agent\\output\\Run\\Release\\Release\\sync_call_system.exe", "/c E:\\work\\ds1\\legendserver\\branches\\branch_agent\\output\\Run\\Release\\2_StartAllRelease.bat", "E:\\work\\ds1\\legendserver\\branches\\branch_agent\\output\\Run\\Release\\");
	//nret = CreateDetachProcess("C:\\Windows\\System32\\cmd.exe", "/min /b E:\\work\\ws3\\legendserver\\output\\Run\\eslib\\Debug\\Redis\\redis-server.exe redis.windows.conf", "E:\\work\\ws3\\legendserver\\output\\Run\\eslib\\Debug\\Redis\\");
	//nret = CreateDetachProcess("redis-server.exe", "/min /b E:\\work\\ws3\\legendserver\\output\\Run\\eslib\\Debug\\Redis\\redis-server.exe redis.windows.conf", "E:\\work\\ws3\\legendserver\\output\\Run\\eslib\\Debug\\Redis\\");
	//nret = CreateDetachProcess("C:\\Windows\\System32\\cmd.exe", "/c E:\\work\\ws2\\legendserver\\output\\Run\\1_StartAllDebug.bat", "C:\\Windows\\System32\\");
	//nret = CreateDetachProcess("C:\\Windows\\System32\\cmd.exe", "/c E:\\work\\ws2\\legendserver\\output\\Run\\1_StartAllDebug.bat", "E:\\work\\ws2\\legendserver\\output\\Run\\");
	//nret = CreateDetachProcess("C:\\Windows\\System32\\cmd.exe", "/c 1_StartAllDebug.bat", "E:\\work\\ws2\\legendserver\\output\\Run\\");
	//nret = CreateDetachProcess("C:\\Windows\\System32\\cmd.exe", "/c start_exe.bat", "E:\\work\\ws2\\legendserver\\output\\Run\\eslib\\Debug\\");
	//nret = CreateDetachProcess("E:\\work\\ws2\\legendserver\\output\\Run\\eslib\\sync_call_system.exe", " ", workingDir.c_str());
	//nret = CreateDetachProcess("C:\\Windows\\System32\\cmd.exe", "/c E:\\work\\ws2\\legendserver\\output\\Run\\eslib\\Debug\\start_exe.bat", "E:\\work\\ws2\\legendserver\\output\\Run\\eslib\\Debug\\");
	//nret = CreateDetachProcess("C:\\Windows\\System32\\cmd.exe", "/c E:\\work\\ws2\\legendserver\\output\\Run\\start_exe.bat loginserverD.exe", "E:\\work\\ws2\\legendserver\\output\\Run\\");
	//nret = CreateDetachProcess("C:\\Windows\\System32\\cmd.exe", "/c E:\\work\\ws2\\legendserver\\output\\Run\\start_exe.bat WorldServerD.exe", "E:\\work\\ws2\\legendserver\\output\\Run\\");
	//nret = CreateDetachProcess("C:\\Windows\\System32\\cmd.exe", "/c E:\\work\\ws2\\legendserver\\output\\Run\\eslib\\start_exe.bat loginserverD.exe", "E:\\work\\ws2\\legendserver\\output\\Run\\eslib\\");

	nret = CreateDetachProcess("D:\\game\\agent_server\\sync_call_system.exe", "/c D:\\game\\agent_server\\start_agent.bat", "D:\\game\\agent_server\\");

	cout << "nret:" << nret << endl;

	return 0;
}
