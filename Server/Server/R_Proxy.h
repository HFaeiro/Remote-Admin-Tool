#pragma once
#include "Packet_Handler.h"

class R_Proxy
{
public:
	R_Proxy();
	~R_Proxy();


	int Initiate_Proxy(Packet_Handler*);


private:
	struct SocketData
	{
		Packet_Handler* _Handle;
		SOCKET ListenSocket;
		SOCKET ClientSocket;
		SOCKADDR_IN BrowserInfo;
		SOCKET BrowserSocket;
		char address[256];
		int port;

	};

	static UINT BrowserToClient(LPVOID);
//	static SOCKET ConnectToWebServer(LPVOID);
	static int GetAddressAndPort(char*, char* );
	static int Lrecv(SOCKET X, char* buff, int size);
	static int Lsend(SOCKET X, char* buff, int size);
};

