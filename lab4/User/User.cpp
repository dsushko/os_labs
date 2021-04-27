#pragma warning(disable:4996)

#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <string>

int main()
{

	PROCESS_INFORMATION pi;
	PROCESS_INFORMATION pi1;
	STARTUPINFOA si;
	STARTUPINFOA si1;
	PROCESS_INFORMATION paintpi;
	STARTUPINFOA paintsi;
	PROCESS_INFORMATION notepadpi;
	STARTUPINFOA notepadsi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	ZeroMemory(&si1, sizeof(si1));
	si1.cb = sizeof(si1);
	ZeroMemory(&pi1, sizeof(pi1));

	ZeroMemory(&paintsi, sizeof(paintsi));
	paintsi.cb = sizeof(paintsi);
	ZeroMemory(&paintpi, sizeof(paintpi));

	ZeroMemory(&notepadsi, sizeof(notepadsi));
	notepadsi.cb = sizeof(notepadsi);
	ZeroMemory(&notepadpi, sizeof(notepadpi));

	const char* var_to_kill = "PROC_TO_KILL=mspaint,notepad";
	putenv(var_to_kill);

	CreateProcessA("c:\\Windows\\System32\\mspaint.exe", NULL, NULL, NULL, FALSE, NULL, NULL, "c:\\Windows\\System32\\", &paintsi, &paintpi);
	Sleep(1000);

	CreateProcessA("c:\\Windows\\System32\\notepad.exe", NULL, NULL, NULL, FALSE, NULL, NULL, "c:\\Windows\\System32\\", &notepadsi, &notepadpi);
	Sleep(1000);

	const char* cpath_with_arg = "С:\\Killer.exe --name Telegram"; //telega
	char* path_with_arg = new char[strlen(cpath_with_arg)];
	for (int i = 0; i < strlen(cpath_with_arg); i++) {
		path_with_arg[i] = cpath_with_arg[i];
	}

	if (!CreateProcessA("c:\\Killer.exe", path_with_arg, NULL, NULL, FALSE, NULL, NULL, "c:\\", &si, &pi))
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
		return 0;
	}


	const char* cpath_with_arg1 = "С:\\Killer.exe --id 5616"; //BatteryCare
	char* path_with_arg1 = new char[strlen(cpath_with_arg1)];
	for (int i = 0; i < strlen(cpath_with_arg1); i++) {
		path_with_arg1[i] = cpath_with_arg1[i];
	}

	if (!CreateProcessA("c:\\Killer.exe", path_with_arg1, NULL, NULL, FALSE, NULL, NULL, "c:\\", &si1, &pi1))
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
		return 0;
	}
	WaitForSingleObject(pi1.hProcess, INFINITE);

	CloseHandle(pi1.hProcess);
	CloseHandle(pi.hThread);

	
	std::cin.get();
	return 0;
}
