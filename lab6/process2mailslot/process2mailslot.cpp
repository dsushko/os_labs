#include "iostream"
#include "math.h"
#include <windows.h>
#include <stdio.h>
#include <conio.h>


using namespace std;
int main()
{
	setlocale(LC_ALL, "Russian");
	unsigned int N;
	int i, j;
	HANDLE MailslotRecv;
	HANDLE MailslotSend;
	DWORD BytesWritten;
	CHAR ServerName[256];
	char buffer[1024];
	DWORD dwRead;

	if ((MailslotRecv = CreateMailslot(L"\\\\.\\Mailslot\\slot2", 0, MAILSLOT_WAIT_FOREVER, NULL)) == INVALID_HANDLE_VALUE)
	{
		printf("Ошибка при создании ПЯ %d\n", GetLastError());
		return 0;
	}

	Sleep(1500);

	if ((MailslotSend = CreateFile(L"\\\\.\\Mailslot\\slot1", GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
	{
		printf("Ошибка открытия слота %d\n", GetLastError());
		getchar();
		return 0;
	}
	
	if (ReadFile(MailslotRecv, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
	{
		buffer[dwRead] = '\0';
		printf("%s", buffer);

		if (WriteFile(MailslotSend, "Hello Mailslot back!\n", 22, &BytesWritten, NULL) == 0)
		{
			printf("Ошибка отправки %d\n", GetLastError());
			getchar();
			return 0;
		}
	}	

	CloseHandle(MailslotRecv);
	return 0;
}