#pragma once

#include "stdafx.h"




void gameStart();
void Display(HWND g_status, const char* fmt, ...);
DWORD WINAPI TimerThread(LPVOID arg);
void newRound();