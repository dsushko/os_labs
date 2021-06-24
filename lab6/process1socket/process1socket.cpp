#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define _WINSOCK_DEPRECATED_NO_WARNINGS

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
	int result = WSAStartup(MAKEWORD(2, 2), &wsdata);
	if (result != 0) {
		printf("%s", "Unable to initilize windows socket\n");
		_getch();
		return 1;
	}

	struct addrinfo* addrResult = NULL, hints;
	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	result = getaddrinfo(NULL, DEFAULT_PORT, &hints, &addrResult);
	if (result != 0) {
		printf("%s", "Error in getaddrInfo");
		_getch();
		return 1;
	}
	SOCKET listenSocket = INVALID_SOCKET;
	listenSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);

	if (listenSocket == INVALID_SOCKET) {
		printf("%s", "Error in creating socket object\n");
		_getch();
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons(27015);

	if (bind(listenSocket, addrResult->ai_addr,
		(int)addrResult->ai_addrlen) == SOCKET_ERROR) {
		printf("bind failed: %d\n", WSAGetLastError());
		freeaddrinfo(addrResult);
		closesocket(listenSocket);
		WSACleanup();
		_getch();
		return 1;
	}
	freeaddrinfo(addrResult);
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("%s", "Error in listening socket");
		_getch();
		closesocket(listenSocket);
		WSACleanup();
	}
	SOCKET clientSocket = INVALID_SOCKET;
	clientSocket = accept(listenSocket, NULL, NULL);
	if (clientSocket == INVALID_SOCKET) {
		closesocket(listenSocket);
		WSACleanup();
	}

	char recvbuf[DEFAULT_BUFLEN];
	int iRecvResult, iSendResult;
	int recvbuflen = DEFAULT_BUFLEN;

	do {
		iRecvResult = 0;
		iSendResult = 0;
		iRecvResult = recv(clientSocket, recvbuf, recvbuflen, 0);
		if (iRecvResult > 0) {
			printf("Bytes received: %d\n", iRecvResult);
			printf("Received message: %s", recvbuf);
			_getch();
			// send back
			const char* msg2 = "got your test, take a cookie\n\0";
			iSendResult = send(clientSocket, msg2, 30, 0);
			if (iSendResult == SOCKET_ERROR) {
				printf("send failed: %d\n", WSAGetLastError());
				closesocket(clientSocket);
				WSACleanup();
				return 1;
			}
			printf("Bytes sent: %d\n", iSendResult);
		}
		else if (iRecvResult == 0) {
			printf("Connection closing...\n");
		}
		else {
			printf("recv failed: %d\n", WSAGetLastError());
			closesocket(clientSocket);
			WSACleanup();
			return 1;
		}
	} while (iRecvResult > 0);
	_getch();
	return 0;
}