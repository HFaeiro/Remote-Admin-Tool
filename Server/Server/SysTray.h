#pragma once
#include "stdafx.h"
class SysTray
{
public:
	SysTray();
	~SysTray();

	NOTIFYICONDATA nID;


	int AddTray(HICON, BOOL, NOTIFYICONDATA*, HWND, int);
	int HideTray(NOTIFYICONDATA*);
	int UpdateTray(NOTIFYICONDATA*, BOOL, int);
};

