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

	return 0;
}
