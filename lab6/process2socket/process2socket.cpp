#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <conio.h>

#define DEFAULT_PORT        "27015"
#define DEFAULT_BUFLEN        512
#pragma comment(lib, "Ws2_32.lib")

int main() {
	WSADATA wsdata;
	WSAStartup(MAKEWORD(2, 2), &wsdata);
	struct addrinfo* addrResult = NULL, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_UNSPEC;
	int result = 0;
	if (getaddrinfo("127.0.0.1", DEFAULT_PORT,
		&hints, &addrResult)) {
		printf("%s", "Error in getaddrInfo\n");
		WSACleanup();
		_getch();
		return 1;
	}
	SOCKET connectingSocket = INVALID_SOCKET;
	connectingSocket = socket(addrResult->ai_family, addrResult->ai_socktype,
		addrResult->ai_protocol);
	if (connectingSocket == INVALID_SOCKET) {
		printf("%s", "Error in creating socket\n");
		freeaddrinfo(addrResult);
		WSACleanup();
		_getch();
		return 1;
	}

	if (connect(connectingSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen) != 0) {
		closesocket(connectingSocket);
		connectingSocket = INVALID_SOCKET;
		WSACleanup();

	}
	freeaddrinfo(addrResult);

	int recvbuflen = DEFAULT_BUFLEN;
	const char* sendbuf = "this is a test\n\0";
	char recvbuf[DEFAULT_BUFLEN];

	int iResult;

	iResult = send(connectingSocket, sendbuf, 16, 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(connectingSocket);
		WSACleanup();
		return 1;
	}

	printf("Bytes Sent: %ld\n", iResult);


	iResult = shutdown(connectingSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(connectingSocket);
		WSACleanup();
		return 1;
	}

	//while connected
	do {
		iResult = recv(connectingSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);
			printf("Received message: %s", recvbuf);
		}
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());
	} while (iResult > 0);

	_getch();
	return 0;
}