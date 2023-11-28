#pragma once

/*만든 헤더*/
#include "Winsock.h"
#include "PrjClient.h"

// ====== 지윤 ======
#include "Drawing.h"

//* 지안 만든 헤더 *//
#include "Home.h"
#include "Login.h"

// ====== 정호 ======
#include "math.h"

// ====== 연경 ======
#include "Ranking.h"
#include "Chatting.h"
#include "Game.h"
#include <time.h>

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

// ============= 연경 =============
static HWND          g_hRankDlg;      // 랭킹 다이얼로그 핸들
static HWND          g_hChattingDlg;  // 채팅 다이얼로그 핸들
static HWND          g_hTimerStatus;  // 타이머 영역
static HWND          g_hWordStatus;   // 제시어 영역

// ============= 지윤 =============
static HWND			 g_hBtnPenColor;  // [색상 변경] 버튼
static HWND			 g_hLineWidth;    // 선 굵기 콤보박스

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

// ============= 지윤 =============
static int			 g_lineWidth;     // 선 그리기 굵기


/* 지안 (2000번부터) */
// 로그인 사용자 정의 상수
#define ID_LOGIN_BUTTON			2000	// 로그인 버튼
#define ID_ID_INPUT				2001	//로그인 input
#define ID_DUPLICATION_BUTTON	2002	// 중복 확인 버튼
#define ID_BACKHOME_BUTTON		2003	// 돌아가기 버튼

// 로그인 관련 전역 변수
extern _TCHAR		ID_NICKNAME[256];	// 현재 사용자 아이디 전역변수

// 홈 사용자 정의 상수
#define ID_CHANNEL_A_BUTTON		2500	// 채널 A 버튼
#define ID_CHANNEL_B_BUTTON		2501	// 채널 B 버튼
#define ID_CHANNEL_RANDOM_BUTTON 2502	// 랜덤 접속 버튼

// 윈도우 관련 전역 변수
static HWND			hwndLogin;			// 로그인 윈도우 (2000..)
static HWND			hwndHome;			// 홈 화면 윈도우 (2500..)


// ======= 정호 =========
#define TYPE_DRAWELLIPSE 4000			// 메시지 타입 : 타원 그리기
#define WM_DRAWELLIPSE (WM_USER+3)		// 타원 그리기 윈도우 메시지

// 그리기 모드 종류
#define MODE_ERASE 4500
#define MODE_LINE 4501
#define MODE_ELLIPSE 4502
#define MODE_RECTANGLE 4503
#define MODE_TRIANGLE 4504

// 현재 그리기 모드 전역 변수
static int g_currentSelectFigureMode = MODE_LINE;

// 현재 세부 그리기 정보 전역 변수
static DRAW_DETAIL_INFORMATION g_drawDetailInformation;

static HWND g_hFigureSelect; // 그릴 도형 선택하는 부분

static DRAWELLIPSE_MSG g_drawellipsemsg; // 타원 그리기 메시지
//