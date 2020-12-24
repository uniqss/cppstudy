#include "cppcalllua_lib.h"
#include "uniqs_xlsx_reader.h"

int main(int argc, const char** argv)
{
	std::vector<std::pair<std::string, std::vector<std::vector<std::string> > > > vecxlsx;

	if (argc < 2)
	{
		printf("usage: %s xlsxFileName \n", argv[0]);
		return -1;
	}

	read_xlsx(argv[1], vecxlsx);

	int ret = 0;
	for (const auto& it : vecxlsx)
	{
		if (it.second.size() > 5)
		{
			ret = ProcessOneSheet(argv[1], it.first, it.second[1], it.second[2], it.second[4]);
		}
	}

#if 0
	std::string xlsxName = "testXlsxName";
	std::string sheetName = "testSheetName";

	std::vector<std::string> vecNames = { "aaaaa", "bbbbb", "cccc", "ddddd", "1111", "2222" };
	std::vector<std::string> vecTypes = { "string", "string", "string", "string", "int", "int" };
	std::vector<std::string> vecDescriptions = { "hello", "world", "", "ddddddddd", "ddd111", "ddd2222" };

	int ret = ProcessOneSheet(xlsxName, sheetName, vecNames, vecTypes, vecDescriptions);

	printf("ret:%d\n", ret);
#endif

	return 0;
}
