#pragma once

/*만든 헤더*/
#include "Winsock.h"
#include "PrjClient.h"
#include "Drawing.h"
#include "DrawingAdd.h"

/*리소스*/
#include "resource.h" // 그림판 다이어로그창

/*식별자*/
#define SERVERIP4  _T("192.168.45.186")
#define SERVERIP6  _T("::1")
#define SERVERPORT  9000

#define TYPE_CHAT     1000              // 메시지 타입: 채팅
#define TYPE_DRAWLINE 1001              // 메시지 타입: 선 그리기
#define TYPE_ERASEPIC 1002              // 메시지 타입: 그림 지우기

#define WM_DRAWLINE (WM_USER+1)         // 사용자 정의 윈도우 메시지(1)
#define WM_ERASEPIC (WM_USER+2)         // 사용자 정의 윈도우 메시지(2)