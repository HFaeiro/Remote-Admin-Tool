#include "Initialization.h"


Initialization::Initialization()
{

	sInit._cHandler = new Client_Handler;

	result = NULL;
	//memset(result, 0, sizeof(addrinfo));
	ptr = NULL;
	data.port = NULL;
	wcstombs(data.URL, L"\0", 256);
}


Initialization::~Initialization()
{



}
int Initialization::Initialize(Packet_Handler* _Handle, Initialization *Init)
{
	int iResult = NULL;
	_Handle->si.ListenSocket = INVALID_SOCKET;
	_Handle->si.ConnectionCounter = 0;


	if ((iResult = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
		printf("WSAStartup Failed: %d\n", iResult);
		return 1;
	}
	printf("WinSock Initialized...\n");



	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family =  AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	//	hints.ai_flags = AI_PASSIVE;
	std::stringstream ss;
	ss << Init->data.port;
	std::string str = ss.str();

	if ((iResult = getaddrinfo(Init->data.URL, str.c_str(), &hints, &result)) != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}
	struct in_addr addr;
	((struct sockaddr_in *)(result->ai_addr))->sin_addr.s_addr = htonl(INADDR_ANY);
	//OutputDebugStringA(inet_ntoa(csin.sin_addr) );

	if ((_Handle->si.ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol)) == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	if ((iResult = bind(_Handle->si.ListenSocket, result->ai_addr, (int)result->ai_addrlen)) == SOCKET_ERROR) {
		//printf("bind failed with error: %d\n", WSAGetLastError());
		WSAGetLastError();
		freeaddrinfo(result);
		closesocket(_Handle->si.ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	if (listen(_Handle->si.ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(_Handle->si.ListenSocket);
		WSACleanup();
		return 1;
	}
	printf("Listening....\n");
	//SetWindowText(0, L"Server Listening...");
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Static_Listen, (LPVOID)_Handle, NULL, NULL);


	return 0;
}
void Initialization::Static_Listen(Packet_Handler* _Handle)
{
	_Handle->si.ConnectionCounter = 0;

	while (_Handle->start)
	{
		
		struct sockaddr_in ClientInfo = { 0 };
		int size = sizeof(ClientInfo);
		socklen_t sSize = sizeof(ClientInfo);
		if ((_Handle->si.ClientSocket = accept(_Handle->si.ListenSocket, (sockaddr*)&ClientInfo, &size)) != INVALID_SOCKET) {
			if (!sInit._cHandler->Handle_Client(_Handle->si.ConnectionCounter, _Handle))
			{
				//OutputDebugStringA(inet_ntoa(ClientInfo.sin_addr));
				_Handle->Add_List_Items(_Handle);
				_Handle->si.ConnectionCounter++;
				if (_Handle->si.ConnectionCounter >= 249)
				{	
					break;
				}
				_Handle->Tray->UpdateTray(&_Handle->Tray->nID, _Handle->start, _Handle->si.ConnectionCounter);
			}

		}

	}
}