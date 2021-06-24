#include <iostream>
#include <Windows.h>

int main() {
	HANDLE hPipe;
	DWORD dwWritten;

	Sleep(1000);

	char buffer[1024];
	DWORD dwRead;

	hPipe = CreateFile(TEXT("\\\\.\\pipe\\Pipe"),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (hPipe != INVALID_HANDLE_VALUE)
	{
		WriteFile(hPipe,
			"Hello Pipe\n",
			12,   
			&dwWritten,
			NULL);

		Sleep(1000);
		ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL);
		buffer[dwRead] = '\0';
		printf("%s", buffer);

		CloseHandle(hPipe);
	}
	


	return (0);
}