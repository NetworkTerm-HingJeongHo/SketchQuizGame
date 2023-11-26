#pragma once

/*만든 헤더*/
#include "Winsock.h"
#include "PrjClient.h"
#include "Drawing.h"

/*추가로 사용하는 라이브러리*/
#include "math.h"

/*리소스*/
#include "resource.h" // 그림판 다이어로그창

/*식별자*/
#define SERVERIP4  _T("127.0.0.1")
#define SERVERIP6  _T("::1")
#define SERVERPORT  9000

/* 윈도우 관련 전역 변수 */
static HINSTANCE     g_hInstance;     // 프로그램 인스턴스 핸들
static HWND          g_hBtnSendFile;  // [파일 전송] 버튼
static HWND          g_hBtnSendMsg;   // [메시지 전송] 버튼
static HWND          g_hEditStatus;   // 각종 메시지 출력 영역
static HWND          g_hBtnErasePic;  // [그림 지우기] 버튼
static HWND          g_hDrawWnd;      // 그림을 그릴 윈도우
static HWND			 g_hMainWindow;   // 처음 윈도우창 핸들
static HWND			 g_hDialog;       // 그림판 다이어로그 핸들

// 다이얼로그 상태 플래그
static bool          g_bDialogVisible = false;

/* 통신 관련 전역 변수 */
static volatile bool g_isIPv6;        // IPv4 or IPv6 주소
static char          g_ipaddr[64];    // 서버 IP 주소(문자열)
static int           g_port;          // 서버 포트 번호
static volatile bool g_isUDP;         // TCP or UDP 프로토콜
static HANDLE        g_hClientThread; // 스레드 핸들
static volatile bool g_bCommStarted;  // 통신 시작 여부
static SOCKET        g_sock;          // 클라이언트 소켓
static HANDLE        g_hReadEvent;    // 이벤트 핸들(1)
static HANDLE        g_hWriteEvent;   // 이벤트 핸들(2)

/* 메시지 관련 전역 변수 */
static CHAT_MSG      g_chatmsg;       // 채팅 메시지
static DRAWLINE_MSG  g_drawlinemsg;   // 선 그리기 메시지
static int           g_drawcolor;     // 선 그리기 색상
static ERASEPIC_MSG  g_erasepicmsg;   // 그림 지우기 메시지
static DRAWELLIPSE_MSG g_drawellipsemsg; // 타원 그리기 메시지