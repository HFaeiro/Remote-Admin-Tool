#pragma once
#include "Remote_Handler.h"


class Client_Handler
{
public:
	Client_Handler();
	~Client_Handler();
	int Handle_Client(int, Packet_Handler*);
	int Refresh_Client_List(Packet_Handler*);
	void Multi_Client_Helper(Packet_Handler* _Handle, int iDetails, int iPacketType, int i);
	//Remote_Handler* _rHandle = new Remote_Handler();
private:
	wchar_t *GetLangFromID(int);
public:

};

