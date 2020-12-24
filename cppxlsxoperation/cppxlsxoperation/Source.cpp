#include "uniqs_xlsx_reader.h"
#include<iostream>
using namespace std;
int main()
{
	std::vector<std::pair<std::string, std::vector<std::vector<std::string> > > > vecxlsx;
	read_xlsx("test.xlsx", vecxlsx);
	for (auto it_sheet : vecxlsx)
	{
		cout << "sheet_name:" << it_sheet.first << endl;
		for (auto it_row : it_sheet.second)
		{
			for (auto it_col : it_row)
			{
				cout << it_col << "\t";
			}
			cout << endl;
		}
	}
	return 0;
}
