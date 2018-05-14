// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

//#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <WinSock2.h>
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <commctrl.h>
// TODO: reference additional headers your program requires here


#include <WS2tcpip.h>
#include <stdio.h>
#include <conio.h>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>


#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "comctl32.lib")

#include "Resource.h"


#define Remote			2
#define Quit			-1
#define Kill			-2
#define Screeny			5
#define Reboot			6
#define RProxy			7