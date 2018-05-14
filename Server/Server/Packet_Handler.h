#pragma once

#include "SysTray.h"

class Packet_Handler
{
public:
	Packet_Handler();
	~Packet_Handler();
	enum  _Packet_Type {Null = 0, File, Remote1, Mouse, Key }Packet_Type;
	int Determine_Packet(Packet_Handler*, _Packet_Type);

	//char* Remote_View(Packet_Handler*, int, char*, CLSID*);
	
	//int Remote_View(Packet_Handler*);



	HWND hWndList;
	LV_COLUMNW lvC;
	LVITEM lvI;
	HINSTANCE hInst;
	HWND mhWnd;

	struct _Clients
	{
		wchar_t *_ClientID = new wchar_t[64];
		wchar_t *_IPAdress = new wchar_t[256];
		wchar_t *_OperatingSystem = new wchar_t[64];
		wchar_t *_Country = new wchar_t[64];


	};

	struct MultiClient
	{
		
		
		SOCKET s[100] = { 0 };
		_Clients* Clients = new _Clients;
		IStream* ScreenShot;
		int ssSize;
	};


	struct _Socketsi
	{
		SOCKET L;
		int picked;
		BOOL IsListen;
		SOCKET ClientSocket;
		MultiClient Connections[250] = { 0 };
		SOCKET ListenSocket;
		int ConnectionCounter;
	}si;

	BOOL SCREENY;
	 int socksize = sizeof(SOCKET);
	 BOOL start;
	 SysTray* Tray = new SysTray();
	 int Clicked_Item;
	//_Clients* Clients = new _CLients[250]; // move to multiclient
	int Add_List_Items(Packet_Handler*);
	int i = 0;
	BYTE *Receive_Full_Packet(Packet_Handler*, int* , void*, bool, int, int);
	//int NumberOfItems; //use connection counter instead
private:

	int File_Handler(Packet_Handler*);


	int Send_Full_Packet(Packet_Handler*);





};


