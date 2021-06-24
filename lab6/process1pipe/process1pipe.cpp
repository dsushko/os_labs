#include <windows.h> 
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <iostream>

using namespace std;

#define BUFSIZE 512
 
int main()
{
	HANDLE hPipe;
	DWORD dwWritten;
	char buffer[1024];
	DWORD dwRead;


	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\Pipe"),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,   
		1,
		1024 * 16,
		1024 * 16,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	
	
	PROCESS_INFORMATION pi;
	STARTUPINFOA si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	CreateProcessA("c:\\folder\\process2pipe.exe", NULL, NULL, NULL, FALSE, NULL, NULL, "c:\\folder\\", &si, &pi);
	

	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)   
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				buffer[dwRead] = '\0';

				printf("%s", buffer);

				WriteFile(hPipe,
					"Hello Pipe in answer!\n",
					21,   
					&dwWritten,
					NULL);
			}
		}

		DisconnectNamedPipe(hPipe);
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	Sleep(1000);
	return 0;
}