#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>
using std::cout;
using std::endl;

int main(int argc, const char** argv) {
	std::stringstream ss;
	for (size_t i = 1; i < argc; ++i) {
		ss << " " << argv[i];
	}

	std::string cmd = ss.str();
	cout << "cmd:" << cmd << endl;

	int ret = std::system(cmd.c_str());

	return ret;
}
