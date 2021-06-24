#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

#include <string>
#include <iostream>
#include <Windows.h>

using namespace std;

int main()
{
	const char* var_to_kill = "PROC_DATA=0";
	putenv(var_to_kill);

	std::string procdata;

	char const* temp = getenv("PROC_DATA");
	if (temp != NULL)
	{
		procdata = std::string(temp);
	}
	cout << procdata << "\n";

	int num = stoi(procdata);

	num += 100;
	cout << num << "\n";
	string new_num = "PROC_DATA=" + to_string(num);
	const char* newval = new_num.c_str();
	putenv(newval);


	PROCESS_INFORMATION pi;
	STARTUPINFOA si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	CreateProcessA("c:\\folder\\process2envvar.exe", NULL, NULL, NULL, FALSE, NULL, NULL, "c:\\folder\\", &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
	Sleep(1000);


	char const* temp1 = getenv("PROC_DATA");
	if (temp != NULL)
	{
		procdata = std::string(temp1);
	}
	cout << procdata << "\n";

	return 0;
}
