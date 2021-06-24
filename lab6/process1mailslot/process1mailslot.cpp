#include "iostream"
#include "math.h"
#include <windows.h>

using namespace std;
int main()
{
	setlocale(LC_ALL, "Russian");
	HANDLE MailslotRecv;
	HANDLE MailslotSend;
	char buffer[1024];
	DWORD dwRead;
	DWORD dwWritten;
	unsigned int N = 0;
	DWORD NumberOfBytesRead;
	if ((MailslotRecv = CreateMailslot(L"\\\\.\\Mailslot\\slot1", 0, MAILSLOT_WAIT_FOREVER, NULL)) == INVALID_HANDLE_VALUE)
	{
		printf("Ошибка при создании ПЯ %d\n", GetLastError());
		return 0;
	}

	PROCESS_INFORMATION pi;
	STARTUPINFOA si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	CreateProcessA("c:\\folder\\process2mailslot.exe", NULL, NULL, NULL, FALSE, NULL, NULL, "c:\\folder\\", &si, &pi);
	Sleep(1000);

	if ((MailslotSend = CreateFile(L"\\\\.\\Mailslot\\slot2", GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
	{
		printf("Ошибка открытия слота %d\n", GetLastError());
		getchar();
		return 0;
	}


	if (WriteFile(MailslotSend, "Hi Mailslot!\n", 14, &dwWritten, NULL) == 0)
	{
		printf("Ошибка отправки %d\n", GetLastError());
		getchar();
		return 0;
	}

	Sleep(1500);

	if (ReadFile(MailslotRecv, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
	{
		buffer[dwRead] = '\0';

		printf("%s", buffer);
	}
	
	cin.get();

	CloseHandle(MailslotRecv);
	return 0;
}
