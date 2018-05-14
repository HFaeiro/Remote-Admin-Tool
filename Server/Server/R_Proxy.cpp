#include "stdafx.h"
#include "R_Proxy.h"


R_Proxy::R_Proxy()
{
}


R_Proxy::~R_Proxy()
{
}

int R_Proxy::Initiate_Proxy(Packet_Handler* _Handle)
{

	
	SocketData *sData = new SocketData;
	sData->_Handle = _Handle;
	unsigned long ul = 1;
	if (sData->_Handle->si.L) {
		int size = sizeof(sData->BrowserInfo);
		sData->BrowserSocket = accept(sData->_Handle->si.L, (sockaddr*)&sData->BrowserInfo, &size);
		ioctlsocket(sData->BrowserSocket, FIONBIO, &ul);


	}
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)BrowserToClient, sData, NULL, NULL);


	return 0;
}

UINT R_Proxy::BrowserToClient(LPVOID pParam)
{
	SocketData* sData = new SocketData;
	sData = (SocketData*)pParam;
	sData->ListenSocket = sData->_Handle->si.L;
	unsigned long ul = 1;
	int retval, iport;

	if (sData->BrowserSocket == INVALID_SOCKET) { closesocket(sData->BrowserSocket); return -5; }
	char Buffer[1000000] = { '\0' };

	

	retval = Lrecv(sData->BrowserSocket, Buffer, 0x15000);
	if (retval == SOCKET_ERROR || retval == -0) { closesocket(sData->BrowserSocket); return retval; }

	if ((iport = GetAddressAndPort(Buffer, sData->address)) != 1)
	{
		if (iport != -1) sData->port = iport;
		else return -1;
	}
	else sData->port = 80;
	
	sData->ClientSocket = sData->_Handle->si.Connections[sData->_Handle->si.picked].s[7]; 
	int addrlen = strlen(sData->address);
	addrlen += 1;
	sData->address[addrlen] = '\0';
	retval = send(sData->ClientSocket, (char*)&addrlen, 4, 0);
	if (retval == SOCKET_ERROR)
		return retval;
	retval = send(sData->ClientSocket, sData->address,addrlen, 0);
	if (retval == SOCKET_ERROR)
		return retval;
	retval = send(sData->ClientSocket, (char*)&sData->port, 4, 0);
	if (retval == SOCKET_ERROR)
		return retval;






	if (sData->port == 80)
	{
		ioctlsocket(sData->ClientSocket, FIONBIO, &ul);
		retval = send(sData->ClientSocket, Buffer, strlen(Buffer), 0);
		if (retval == 0 || retval == -1)
			return retval;




		int c = 0;
		for (;;)
		{
			fd_set broRecv;
			timeval conTime;
			conTime.tv_sec = 2;
			conTime.tv_usec = 0;

			int new_sd;
			FD_ZERO(&broRecv);
			FD_SET(sData->BrowserSocket, &broRecv);
			FD_SET(sData->ClientSocket, &broRecv);
			int sel = select(NULL, &broRecv, NULL, NULL, &conTime);
			ULONG bytes_available;
			if (sel > 0)
			{
				if (c >= 10) return NULL;
				if (Buffer)
					memset(Buffer, 0, strlen(Buffer));
				if (FD_ISSET(sData->BrowserSocket, &broRecv)) {
					ioctlsocket(sData->BrowserSocket, FIONREAD, &bytes_available);
					int bAvail = bytes_available;
					if (!bAvail) bAvail = 1;
					retval = recv(sData->BrowserSocket, Buffer, bAvail, 0);
					if (retval == -1)
						return retval;
					if (retval != 0)
					{
						retval = send(sData->ClientSocket, Buffer, retval, 0);
					}
					else return retval;

				}
				if (Buffer)
					memset(Buffer, 0, strlen(Buffer));
				if (FD_ISSET(sData->ClientSocket, &broRecv)) {
					ioctlsocket(sData->ClientSocket, FIONREAD, &bytes_available);
					int bAvail = bytes_available;
					if (!bAvail) bAvail = 1;
					retval = recv(sData->ClientSocket, Buffer, bAvail, 0);
					if (retval == -1)
						return retval;
					if (retval != 0)
					{
						retval = send(sData->BrowserSocket, Buffer, retval, 0);
					}
					else return retval;

				}
			}
			else
			{
				c++;

			}
		}
	}
	else
	{
		int OK = 0;
		retval = recv(sData->ClientSocket, (char*)&OK, 4, 0);
		if (OK != 200)
			return retval;
		ioctlsocket(sData->ClientSocket, FIONBIO, &ul);
		char* conSuc = "HTTP/1.1 200 Connection established\r\n\r\n";
		retval = Lsend(sData->BrowserSocket, conSuc, strlen(Buffer));
		if (retval == 0 || retval == -1)
			return retval;
		int c = 0;

		for (;;)
		{
			fd_set broRecv;
			timeval conTime;
			conTime.tv_sec = 5;
			conTime.tv_usec = 0;
			//sizeof(broRecv.fd_count);
			int new_sd;
			FD_ZERO(&broRecv);
			FD_SET(sData->BrowserSocket, &broRecv);
			FD_SET(sData->ClientSocket, &broRecv);
			int sel = select(NULL, &broRecv, NULL, NULL, &conTime);
			ULONG bytes_available;
			if (sel > 0)
			{
				if (c >= 10) return NULL;
				if (Buffer)
					memset(Buffer, 0, strlen(Buffer));
				if (FD_ISSET(sData->BrowserSocket, &broRecv)) {
					ioctlsocket(sData->BrowserSocket, FIONREAD, &bytes_available);
					int bAvail = bytes_available;
					if (!bAvail) bAvail = 1;
					retval = recv(sData->BrowserSocket, Buffer, bAvail, 0);
					if (retval == -1) return retval;
					if (retval != 0)
						retval = send(sData->ClientSocket, Buffer, retval, 0);
					else return retval;
				}
				if (Buffer)
					memset(Buffer, 0, strlen(Buffer));
				if (FD_ISSET(sData->ClientSocket, &broRecv)) {
					ioctlsocket(sData->ClientSocket, FIONREAD, &bytes_available);
					int bAvail = bytes_available;
					if (!bAvail) bAvail = 1;
					retval = recv(sData->ClientSocket, Buffer, bAvail, 0);
					if (retval == -1) return retval;
					if (retval != 0) retval = send(sData->BrowserSocket, Buffer, retval, 0);
					else return retval;

				}
			}
			else c++;
		}




	}
	return 0;
}


int R_Proxy::GetAddressAndPort(char * str, char * address)
{
	char buf[10240], command[512], proto[128], *p, *port, *t, *host, *c;
	int j, iport;
	sscanf(str, "%s%s%s", command, buf, proto);
	p = buf;
	int i = 0;

	if (p)
	{
		t = strstr(buf, "http://");
		if (t)
		{
			p += strlen("http://");
			for (i; i < strlen(p); i++)
				if (*(p + i) == '/') break;
			*(p + i) = 0;
			strcpy(address, p);
			p = strstr(str, "http://");
			for (int j = 0; j < i + strlen("http://"); j++)
				*(p + j) = ' ';
		}
		else
		{
			if (strlen(command) == 7)
			{

				for (i; i < strlen(p); i++)
					if (*(p + i) == ':') break;
				port = (p + i + 1);
				iport = atoi(port);
				*(p + i) = 0;
				strcpy(address, p);


				return iport;
			}
			else
			{
				host = strstr(str, "Host");
				strcpy(address, host);
				return 1;
			}
		}

	}
	else return -1;

	return 1;

}

int R_Proxy::Lrecv(SOCKET X, char * buff, int size)
{
	int retval;
	int i = 0;
	int count = 0;
	for (;;)
	{
		if (count > 20)
			return i;
		retval = recv(X, buff + i, size, 0);
		int err = WSAGetLastError();
		if (retval == -1)
		{
			if (err == WSAEWOULDBLOCK)
			{
				Sleep(10);
				count++;
				continue;
			}
			return i;
		}
		if (retval == 0)
			return i;
		count--;
		i += retval;
	}
	return 0;
}

int R_Proxy::Lsend(SOCKET X, char* buff, int size)
{
	int retval;
	int i = 0;
	int count = 0;
	for (;;)
	{
		if (count > 20) return i;
		retval = send(X, buff + i, size - i, 0);
		int err = WSAGetLastError();
		if (retval == -1)
		{
			if (err == WSAEWOULDBLOCK)
			{
				Sleep(200);
				count++;
				continue;
			}
			return i;
		}
		if (retval == size)
			return retval;
		count--;
		i += retval;
	}
}