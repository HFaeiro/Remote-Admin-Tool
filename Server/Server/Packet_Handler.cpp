#include "Packet_Handler.h"


Packet_Handler::Packet_Handler()
{
	// std::fill(si.Connections, si.Connections - sizeof(si.Connections), NULL);
	si.ConnectionCounter = NULL;
	start = FALSE;
	hWndList = NULL;
	SCREENY = FALSE;
	si.IsListen = 0;
	si.L = NULL;
	si.picked = NULL;
}
Packet_Handler::~Packet_Handler()
{
}
int Packet_Handler::Determine_Packet(Packet_Handler* _Handle, _Packet_Type Type)
{
	switch (Type)
	{
		//	case Message:	return Message_Handler(_Handle);
	//case File:		return File_Handler(_Handle);
	case Remote:
	default:		return 1;
	}
	return 0;
}
int Packet_Handler::File_Handler(Packet_Handler* _Handle)
{
	//int Check;
	//int BuffLength;
/*
	if (recv(_Handle->si.Connections[_Handle->si.ConnectionCounter - 1], (char*)&BuffLength, sizeof(int), 0) != INVALID_SOCKET)
	{
		if (BuffLength != 0)
		{
			Receive_Full_Packet(_Handle, BuffLength);

		}
	}
	****************************
	printf("File Downloaded");
	FILE *TempFile;
	TempFile = fopen("Test.exe", "wb");
	fwrite(Buff, sizeof(Buff), sizeof(Buff), TempFile);
	fclose(TempFile);
	*******************************
	*/
	return 0;
}
int Packet_Handler::Send_Full_Packet(Packet_Handler* _Handle)
{


	return 0;
}
BYTE* Packet_Handler::Receive_Full_Packet(Packet_Handler * _Handle, int* pSize, void * Buff, bool ispchar, int WhichConnection, int pType)
{
	int rCheck = 0;
	int Size = *pSize;
	rCheck = recv(_Handle->si.Connections[WhichConnection].s[pType], (char*)&Size, 4, 0);
	if (rCheck == -1 || rCheck == 0)
	{
		if(_Handle->start)
			MessageBox(NULL, L"Connection Was Closed On Client End", NULL, NULL);
		return NULL;
	}
	*pSize = Size;
	//Buff = new BYTE[Size];
	BYTE* isBuff = new BYTE[Size];
	int PacketsReceived = 0;
	int PacketsLeft = Size;
	int ReceivedPackets = 0;
	
	while (PacketsReceived < Size)
	{
		if (ispchar == FALSE)
		{
			ReceivedPackets = recv(_Handle->si.Connections[WhichConnection].s[pType], (char*)Buff + PacketsReceived, PacketsLeft, 0);

		}
		else
		{
			
			ReceivedPackets = recv(_Handle->si.Connections[WhichConnection].s[pType], (char*)isBuff + PacketsReceived, PacketsLeft, 0);

		}
		if (ReceivedPackets == SOCKET_ERROR)
		{

			return NULL;

		}

		PacketsReceived += ReceivedPackets;
		PacketsLeft -= ReceivedPackets;
	}
	return isBuff;
}
int Packet_Handler::Add_List_Items(Packet_Handler * _Handle)
{


int iSubItem;

		_Handle->lvI.mask = LVIF_TEXT | LVIF_PARAM | LVIF_STATE;//
		_Handle->lvI.state = 0;      //
		_Handle->lvI.stateMask = 0;  //
		_Handle->lvI.iItem = _Handle->si.ConnectionCounter;
		_Handle->lvI.iSubItem = 0;

		_Handle->lvI.pszText = LPSTR_TEXTCALLBACK;
		_Handle->lvI.cchTextMax = 100;
		_Handle->lvI.lParam = (LPARAM)&si.Connections->Clients[_Handle->si.ConnectionCounter];

		if (ListView_InsertItem(_Handle->hWndList, &_Handle->lvI) < 0)
			return NULL;
		NMLVDISPINFOW pLvdi = { 0 };
		pLvdi.hdr.code = LVN_GETDISPINFO;
		pLvdi.item.iItem = _Handle->si.ConnectionCounter;
		for (iSubItem = 1; iSubItem < 4; iSubItem++)
		{
			pLvdi.item.iSubItem = iSubItem;
			SendMessage(_Handle->mhWnd, WM_NOTIFY, IDS_APP_TITLE, (LPARAM)&pLvdi);
			/*ListView_InsertItem(_Handle->hWndList, &lvI);
			ListView_SetItemText(_Handle->hWndList,
				_Handle->si.ConnectionCounter,
				iSubItem,
				LPSTR_TEXTCALLBACK*/
			//);
		}
	
	return 0;
}