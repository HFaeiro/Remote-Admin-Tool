
#include "SysTray.h"


SysTray::SysTray()
{
	
	nID = { 0 };
	nID.cbSize = sizeof(NOTIFYICONDATA);
	//nID.hWnd = hWnd;
	nID.uID = TRAY_ICON;
	nID.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;// |  NIF_SHOWTIP;
	nID.uCallbackMessage = TRAY_NOTIF;
	//nID.hIcon = LoadIcon(hinstance, MAKEINTRESOURCE(IDI_SERVER));
	//nID.uVersion = NOTIFYICON_VERSION_4;
	//wcscpy(nID.szTip, L"Hello\n This works?");
	// nID.szTip = "Hello";
}


SysTray::~SysTray()
{
}

int SysTray::AddTray(HICON hIcon, BOOL Listen, NOTIFYICONDATA* nID, HWND hWnd, int Connections)
{
	nID->hWnd = hWnd;
	nID->hIcon = hIcon;
	std::wstringstream wss;
	std::wstring wStr;
	if (Listen)
		wss << L"Listening\n" << L"Clients: " << Connections;
	else wss << L"Waiting To Listen";
	wStr = wss.str();
	//wchar_t tray[256];
	wcscpy(nID->szTip, wStr.c_str());
	Shell_NotifyIcon(NIM_ADD, nID);
	return 0;
}

int SysTray::HideTray(NOTIFYICONDATA* nID)
{
	Shell_NotifyIcon(NIM_DELETE, nID);
	return 0;
}

int SysTray::UpdateTray(NOTIFYICONDATA* nID, BOOL Listen, int Connections)
{
	std::wstringstream wss;
	std::wstring wStr;
	if(Listen)
		wss << L"Listening\n" << L"Clients: " << Connections;
	else wss << L"Waiting To Listen";
	wStr = wss.str();
	//wchar_t tray[256];
	wcscpy(nID->szTip, wStr.c_str());

	//wcscpy(nID->szTip, Tip);
	Shell_NotifyIcon(NIM_MODIFY, nID);
	return 0;
}