#include "stdafx.h"
#include "Remote_Handler.h"

Remote_Handler::Remote_Handler()
{	
	
	sHandler.tracking = FALSE;
	Gdiplus::GdiplusStartupInput gdiInput;
	ULONG_PTR gdiToken;
	Gdiplus::GdiplusStartup(&gdiToken, &gdiInput, NULL);
	GetEncoderClsid(L"image/jpeg", &sHandler.clsid);
	sHandler.istream = nullptr;
	sHandler.pointy = NULL;
	sHandler.pointx = NULL;
	sHandler.CursorThread = NULL;
	sHandler.KEY = NULL;
	sHandler.MOUSE = FALSE;
	sHandler.KEYB = FALSE;

}
Remote_Handler::~Remote_Handler()
{
	
}
int Remote_Handler::Remote_Administration(Packet_Handler* _Handle)
{


	

	if (sHandler.Remote_hWnd[_Handle->Clicked_Item] == NULL)
	{

		WNDCLASSEXW wcex;
		
		HINSTANCE hInstance = GetModuleHandle(NULL);
		LoadStringW(hInstance, IDC_REMOTEVIEW, szRemoteClass, MAX_LOADSTRING);
		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = Remote_Proc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MAINICON));
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = 0;
		wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_REMOTEVIEW);
		wcex.lpszClassName = szRemoteClass;
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
		RegisterClassExW(&wcex);
		

		sHandler.Remote_hWnd[_Handle->Clicked_Item] = CreateWindow(szRemoteClass, L"Remote View", WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr, hInstance , nullptr);
		ShowWindow(sHandler.Remote_hWnd[_Handle->Clicked_Item], SW_SHOW);
		UpdateWindow(sHandler.Remote_hWnd[_Handle->Clicked_Item]);
		//ShowWindow(sHandler.Remote_hWnd[_Handle->Clicked_Item], SW_HIDE);

		sHandler.Remote_Thread_Counter[_Handle->Clicked_Item] = TRUE;
		sHandler.Remote_Thread_hWnd[_Handle->Clicked_Item] = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Remote_Thread, _Handle, NULL, NULL);
		

	}
	else
		MessageBox(NULL, L"You Already Have Remote View Open For this Client", NULL, NULL);
	


	return 0;
}
int Remote_Handler::CursorPos(HWND hWnd)
{
	int i = Which_Thread(hWnd);
	while (sHandler.tracking)
	{
		if (sHandler.istream != nullptr)
		{
			POINT p;
			GetCursorPos(&p);
			p = ConvertPoints(&p, hWnd, TRUE, i);
			sHandler.pointx = p.x;
			sHandler.pointy = p.y;
		}
		Sleep(150);
	}

	return 0;
}
int Remote_Handler::Which_Thread(HWND hWnd)
{
	for (int i = 0; i < 100; i++)
	{
		if (sHandler.Remote_hWnd[i] == hWnd)
			return i;

	}
	return NULL;
}
POINT Remote_Handler::ConvertPoints(LPPOINT P, HWND hWnd, BOOL CLICK, int i)
{
	
	if (i == NULL)
		i = Which_Thread(hWnd);

	if (sHandler.istream != nullptr)
	{
		POINT p = *P;
		RECT rect;
		GetClientRect(hWnd, &rect);
		if (CLICK == TRUE)
			ScreenToClient(hWnd, &p);
		float wWidth = rect.right - rect.left;
		float wHeight = rect.bottom - rect.top;
		if (sHandler.iWidth[i] == NULL)
		{
			Gdiplus::Image* image = Gdiplus::Image::FromStream(sHandler.istream);
			sHandler.iWidth[i] = image->GetWidth();
			sHandler.iHeight[i] = image->GetHeight();
			delete image;
		}
		float pointx = p.x;
		float pointy = p.y;
		if (CLICK == TRUE)
		{
			p.x = (pointx * sHandler.iWidth[i]) / wWidth;
			p.y = (pointy * sHandler.iHeight[i]) / wHeight;
		}
		else
		{
			p.x = (pointx * wWidth) / sHandler.iWidth[i];
			p.y = (pointy * wHeight) / sHandler.iHeight[i];
		}

		
		return p;
	}
	
}
LRESULT CALLBACK Remote_Handler::Remote_Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_CREATE:
	
		break;
	case WM_NOTIFY:
		break;
	case WM_MOUSEMOVE:
	{
		if (sHandler.MOUSE)
		{
			if (!sHandler.tracking)
			{
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(TRACKMOUSEEVENT);
				tme.dwFlags = TME_HOVER | TME_LEAVE;
				tme.dwHoverTime = 1;
				tme.hwndTrack = hWnd;
				TrackMouseEvent(&tme);
				sHandler.tracking = TRUE;
			}
		}
		
	}break;
	case WM_MOUSEHOVER:
	{
		if (sHandler.istream != nullptr)
		{
			
			sHandler.CursorThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CursorPos, hWnd, NULL, NULL);
		}
		break;
	}
	case WM_MOUSELEAVE:
	{
		
		sHandler.pointx = NULL;
		sHandler.pointy = NULL;
		sHandler.tracking = FALSE;
		/*if (sHandler.CursorThread != NULL)
		{
			WaitForSingleObject(sHandler.CursorThread, INFINITE);
			CloseHandle(sHandler.CursorThread);
		}*/
		break;
	}
	case WM_KEYDOWN:
		sHandler.KEYB = TRUE;
		sHandler.KEY = wParam;
		break;
	case WM_KEYUP:
		sHandler.KEYB = FALSE;
		break;
	case WM_LBUTTONDOWN:
	{
		if (sHandler.MOUSE)
			sHandler.MDOWN = 1;
		break;
	}
	case WM_LBUTTONUP:
	{
		
		if(sHandler.MOUSE)
			sHandler.MDOWN = 2;
		break;
	}
	case WM_RBUTTONDOWN:
	{
		if (sHandler.MOUSE)
			sHandler.MDOWN = 3;
		break;
	}
	case WM_RBUTTONUP:
		if (sHandler.MOUSE)
			sHandler.MDOWN = 4;
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_SENDCLICK:
		{
			if (!sHandler.MOUSE)
			{
				HMENU hMenu = GetMenu(hWnd);
				MENUITEMINFO mItem;
				mItem.cbSize = sizeof(MENUITEMINFO);
				mItem.fMask = MIIM_TYPE | MIIM_DATA;
				mItem.dwTypeData = NULL;
				GetMenuItemInfo(hMenu, IDM_SENDCLICK, false, &mItem);
				mItem.dwTypeData = L"Don't Send";
				int check = SetMenuItemInfo(hMenu, IDM_SENDCLICK, false, &mItem);
				sHandler.MOUSE = TRUE;
			}
			else
			{
				HMENU hMenu = GetMenu(hWnd);
				MENUITEMINFO mItem;
				mItem.cbSize = sizeof(MENUITEMINFO);
				mItem.fMask = MIIM_TYPE | MIIM_DATA;
				mItem.dwTypeData = NULL;
				GetMenuItemInfo(hMenu, IDM_SENDCLICK, false, &mItem);
				mItem.dwTypeData = L"Send MOUSE";
				int check = SetMenuItemInfo(hMenu, IDM_SENDCLICK, false, &mItem);
				sHandler.MOUSE = FALSE;
			}
		}
		break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{

		PAINTSTRUCT ps;
		sHandler.hdc = BeginPaint(hWnd, &ps);
		RECT rect;
		int width = 0;
		int height = 0;
		if (GetWindowRect(hWnd, &rect))
		{
			width = rect.right - rect.left;
			height = rect.bottom - rect.top;
		}

		if (sHandler.istream != nullptr)
		{
			
			Gdiplus::Graphics graphics(sHandler.hdc);
			Gdiplus::Image* image = Gdiplus::Image::FromStream(sHandler.istream);
			
			Gdiplus::Rect destRect(0, 0, width -15, height - 60);
			graphics.DrawImage(image, destRect);

			delete image;

		}
		
		if (sHandler.cInfo.hCursor != NULL)
		{
			POINT p;
			p.x = sHandler.cInfo.ptScreenPos.x;
			p.y = sHandler.cInfo.ptScreenPos.y;
			p = ConvertPoints(&p, hWnd, FALSE, NULL);
			DrawIcon(sHandler.hdc, p.x, p.y, sHandler.cInfo.hCursor);
		}
		
		EndPaint(hWnd, &ps);
		
	}

	break;
	case WM_DESTROY:
	{
		int i = Which_Thread(hWnd);
		sHandler.Remote_Thread_Counter[i] = FALSE;
		WaitForSingleObject(sHandler.Remote_Thread_hWnd[i], INFINITE);
		if (sHandler.Remote_Thread_hWnd[i] != NULL)
			CloseHandle(sHandler.Remote_Thread_hWnd[i]);
		sHandler.Remote_hWnd[i] = NULL;
		sHandler.MOUSE = FALSE;
		CloseWindow(hWnd);
	}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}



	return 0;
	
}

BYTE* Remote_Handler::Remote_Thread(Packet_Handler* _Handle)
{
	int p;
	if (_Handle->SCREENY)
	{
		p = 5;
	}
	else p = 2;
	//Packet_Handler::_Packet_Type rPacket = Packet_Handler::_Packet_Type::Remote;
	Packet_Handler::_Packet_Type mPacket = Packet_Handler::_Packet_Type::Mouse;
	Packet_Handler::_Packet_Type kPacket = Packet_Handler::_Packet_Type::Key;
	Packet_Handler::_Packet_Type nPacket = Packet_Handler::_Packet_Type::Null;

	int WhichConnection = _Handle->Clicked_Item;
	int rMote = NULL;
//	SendMessage(sHandler.Remote_hWnd[WhichConnection], WM_PAINT, NULL, NULL);
		while(sHandler.Remote_Thread_Counter[WhichConnection] == TRUE || _Handle->SCREENY)
		{
		BOOL StillSend = TRUE;
		send(_Handle->si.Connections[WhichConnection].s[p], (char*)&StillSend, sizeof(BOOL), 0);
			int Size = 0;
			BYTE* Buff = NULL;
			if ((Buff = _Handle->Receive_Full_Packet(_Handle, &Size, Buff, TRUE, WhichConnection, p)) == NULL)
			{
				if (sHandler.Remote_Thread_hWnd[WhichConnection] != NULL)
				{
					CloseHandle(sHandler.Remote_Thread_hWnd[WhichConnection]);
					sHandler.Remote_Thread_hWnd[WhichConnection] = NULL;
					SendMessage(sHandler.Remote_hWnd[WhichConnection], WM_CLOSE, 0, 0);
				}
				break;

			}
			if (_Handle->SCREENY)
			{
				_Handle->si.Connections[WhichConnection].ssSize = Size;
				return Buff;
			}
				if (RecvCursorInfo(_Handle, WhichConnection) == 1)
				{
					if (sHandler.Remote_Thread_hWnd[WhichConnection] != NULL)
					{
						CloseHandle(sHandler.Remote_Thread_hWnd[WhichConnection]);
						sHandler.Remote_Thread_hWnd[WhichConnection] = NULL;
						SendMessage(sHandler.Remote_hWnd[WhichConnection], WM_CLOSE, 0, 0);
					}
					break;
				}

				if (sHandler.MOUSE)
				{

					send(_Handle->si.Connections[WhichConnection].s[2], (char*)&kPacket, sizeof(Packet_Handler::_Packet_Type), 0);
					send(_Handle->si.Connections[WhichConnection].s[2], (char*)&sHandler.KEY, sizeof(WORD), 0);
					sHandler.KEY = NULL;

					//	send(_Handle->si.Connections[WhichConnection].s[2], (char*)&mPacket, sizeof(Packet_Handler::_Packet_Type), 0);
					POINT p;
					p.x = sHandler.pointx;
					p.y = sHandler.pointy;
					send(_Handle->si.Connections[WhichConnection].s[2], (char*)&p, sizeof(POINT), 0);
					send(_Handle->si.Connections[WhichConnection].s[2], (char*)&sHandler.MDOWN, sizeof(int), 0);
					sHandler.pointy = NULL;
					sHandler.pointx = NULL;
					if (sHandler.MDOWN == 2 | sHandler.MDOWN == 4)
						sHandler.MDOWN = 0;

				}
				else
					send(_Handle->si.Connections[WhichConnection].s[2], (char*)&nPacket, sizeof(Packet_Handler::_Packet_Type), 0);
			

			if (CreateStreamOnHGlobal(NULL, true, &sHandler.istream) == S_OK)
			{
				ULONG Bytes;
				if (sHandler.istream->Write(Buff, Size, &Bytes) == S_OK)
				{
					LARGE_INTEGER li;
					li.QuadPart = 0;
					if (sHandler.istream->Seek(li, STREAM_SEEK_SET, NULL) == S_OK)
					{

						InvalidateRect(sHandler.Remote_hWnd[WhichConnection], NULL, TRUE);
						//GetLastError();
						///BeginPaint
						UpdateWindow(sHandler.Remote_hWnd[WhichConnection]);
				//		SendMessage(sHandler.Remote_hWnd[WhichConnection], WM_PAINT, NULL, NULL);
					//RedrawWindow(sHandler.Remote_hWnd[WhichConnection], NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);



					}
				}
			}
			delete Buff;
			Sleep(75);
			sHandler.istream->Release();
			sHandler.istream = nullptr;
		}

	return 0;
}
int Remote_Handler::RecvCursorInfo(Packet_Handler* _Handle, int WhichConnection)
{
	int Check = 0;
	Check =	recv(_Handle->si.Connections[WhichConnection].s[2], (char*)&sHandler.sP, sizeof(POINT), 0);
	if (Check == 0 || Check == -1)
		return 1;
	ZeroMemory(&sHandler.cInfo, sizeof(CURSORINFO));
	//sHandler.cInfo.cbSize = sizeof(CURSORINFO);
	//GetCursorInfo(&sHandler.cInfo);
	recv(_Handle->si.Connections[WhichConnection].s[2], (char*)&sHandler.cInfo, sizeof(CURSORINFO), 0);
	sHandler.cInfo.ptScreenPos.x = sHandler.sP.x;
	sHandler.cInfo.ptScreenPos.y = sHandler.sP.y;

	return 0;
}
int Remote_Handler::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{

	UINT num = 0;
	UINT size = 0;

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;
	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;
	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);
	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	}
	free(pImageCodecInfo);
	return 0;
}
int Remote_Handler::IsRemote()
{
	for (int i = 0; i < 100; i++)
	{
		if (sHandler.Remote_Thread_Counter[i] == TRUE) {
			return 0;
		}

	}
	return 1;
}

