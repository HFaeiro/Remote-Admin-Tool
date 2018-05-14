#pragma once

#include "Packet_Handler.h"
#define MAX_LOADSTRING 100
class Remote_Handler
{
public:
	Remote_Handler();

	~Remote_Handler();
	WCHAR szRemoteClass[MAX_LOADSTRING];
	
	int Remote_Administration(Packet_Handler*); //eventually need which subitem was clicked
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	int IsRemote();
	static BYTE* Remote_Thread(Packet_Handler*);
	
private:
	static int CursorPos(HWND);
	
	static LRESULT CALLBACK Remote_Proc(HWND, UINT, WPARAM, LPARAM);
	static int Which_Thread(HWND);
	static int RecvCursorInfo(Packet_Handler*, int);
	static POINT ConvertPoints(LPPOINT, HWND, BOOL, int);
};


static class _sRemote_Handler
{
public:
	DWORD MOUSE;
	DWORD KEYB;

	WORD KEY;
	float iWidth[100];
	float iHeight[100];
	BOOL tracking;
	POINT sP;
	CURSORINFO cInfo;
	BOOL Remote_Thread_Counter[100];
	HWND Remote_hWnd[100];
	CLSID clsid;
	HDC hdc;
	IStream* istream;
	HANDLE Remote_Thread_hWnd[100];
	HANDLE CursorThread;
	float pointx;
	float pointy;
	struct
	{
		Packet_Handler * _Handle;
	}_Packet_Handler;
	int MDOWN;
}sHandler;