#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)


#include <iostream>
#include <string>

using namespace std;

int main()
{
   
	std::string procdata;

	char const* temp = getenv("PROC_DATA");
	if (temp != NULL)
	{
		procdata = std::string(temp);
		int num = stoi(procdata);

		num--;

		cout << num << "\n";

		string new_num = "PROC_DATA=" + to_string(num);
		const char* newval = new_num.c_str();
		putenv(newval);
	}
	cin.get();
	return 0;
}
