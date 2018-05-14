#pragma once
#include "Packet_Handler.h"
#include "R_Proxy.h"

class Browser_Handler
{
public:
	Browser_Handler();
	~Browser_Handler();

	static int bHandle(Packet_Handler*);

};

