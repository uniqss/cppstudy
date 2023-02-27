#include <iostream>
using std::cout;
using std::endl;

#include "boost/process.hpp"

namespace bp = boost::process;

int main() {
	bp::spawn("E:\\work\\ws2\\legendserver\\output\\Run\\eslib\\Debug\\loginserverD.exe");
	return 0;
}
