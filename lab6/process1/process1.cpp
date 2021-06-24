#include <iostream>
#include <fstream>
#include <Windows.h>

using namespace std;

int main()
{
	ifstream fin("c:\\folder\\input.txt");
	int num = 0;
	fin >> num;
	cout << num << "\n";
	fin.close();

	num += 100;
	ofstream fout("c:\\folder\\input.txt");
	fout << num;
	fout.close();


	PROCESS_INFORMATION pi;
	STARTUPINFOA si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	CreateProcessA("c:\\folder\\process2file.exe", NULL, NULL, NULL, FALSE, NULL, NULL, "c:\\folder\\", &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
	Sleep(1000);


	ifstream fin_("c:\\folder\\input.txt");
	fin_ >> num;
	cout << num << "\n";

	cin.get();
	return 0;
}
