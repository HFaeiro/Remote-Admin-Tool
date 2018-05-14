#include "stdafx.h"
#include "Browser_Handler.h"


Browser_Handler::Browser_Handler()
{
}


Browser_Handler::~Browser_Handler()
{
}

int Browser_Handler::bHandle(Packet_Handler * _Handle)
{
	int rando;
	int connects;
	fd_set Lset;
	timeval tv;

	if (!_Handle->si.L)
	{
		int iResult = NULL, iOptval = 1;

		SOCKADDR_IN addrin;
		ZeroMemory(&addrin, sizeof(addrin));
		addrin.sin_addr.s_addr = AF_UNSPEC;
		addrin.sin_port = htons(5555);
		addrin.sin_family = AF_INET;

		_Handle->si.L = socket(addrin.sin_family, 0, 0);
		WSAGetLastError();
		setsockopt(_Handle->si.L, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char*)&iOptval, sizeof(iOptval));
		if (_Handle->si.L == INVALID_SOCKET) closesocket(_Handle->si.L);

		iResult = bind(_Handle->si.L, (sockaddr*)&addrin, sizeof(addrin));
		if (iResult != 0) return 1;
		if (listen(_Handle->si.L, SOMAXCONN) == SOCKET_ERROR) closesocket(_Handle->si.L);
		unsigned long ul = 1;
		ioctlsocket(_Handle->si.L, FIONBIO, &ul);
	}
	
	for (;;)
	{
		tv.tv_sec = 2;
		tv.tv_usec = 0;
		FD_ZERO(&Lset);
		FD_SET(_Handle->si.L, &Lset); // make option to turn off proxy. ///////////
		int sel = select(NULL, &Lset, NULL, NULL, &tv);
		ULONG bytes_available;
		if (sel > 0) break;
	}
	connects = _Handle->si.ConnectionCounter - 1;
	if (connects > 0)
		_Handle->si.picked = rand() % connects;
	else _Handle->si.picked = connects;


	SendMessage(_Handle->mhWnd, WM_COMMAND, MAKEWPARAM(ID__RPROX, NULL), NULL);

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)bHandle, _Handle, NULL, NULL);
	


	return 0;
}
