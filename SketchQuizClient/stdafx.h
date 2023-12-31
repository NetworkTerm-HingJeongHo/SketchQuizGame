#pragma once

/*만든 헤더*/
#include "Winsock.h"
#include "PrjClient.h"

// ====== 지윤 ======
#include "Drawing.h"

//* 지안 만든 헤더 *//
#include <stdlib.h>
#include "Home.h"
#include "Login.h"
#include "Home_Notice.h"
#include "Home_Pass.h"

// ====== 정호 ======
#include "math.h"
#include "socket.h"

// ====== 연경 ======
#include "Ranking.h"
#include "Chatting.h"
#include "Game.h"
#include <time.h>

/*리소스*/
#include "resource.h" // 그림판 다이어로그창

/*식별자*/
#define SERVERIP4_CHAR_UDP1 "235.7.8.18" // UDP 서버1 (지안)
#define SERVERIP4_CHAR_UDP2 "235.7.8.19" // UDP 서버2 (지안)
#define SERVERIP4_CHAR   "127.0.0.1" //브로드캐스트 대상 주소 및 서버 ip char 버전 (지안)
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

// ============= 지윤 =============
static HWND			 g_hMainWindow;   // 처음 윈도우창 핸들
static HWND			 g_hDialog;       // 그림판 다이어로그 핸들
static HWND			 g_hBtnPenColor;  // [색상 변경] 버튼
static HWND			 g_hLineWidth;    // 선 굵기 콤보박스

// ============= 연경 =============
// 채팅 관련 변수
#define TYPE_NOTY 5000  // 입장, 퇴장, 새 라운드 알림 메시지 출력

static HWND          g_hRankDlg;      // 랭킹 다이얼로그 핸들
static HWND          g_hChattingDlg;  // 채팅 다이얼로그 핸들
static HWND          g_hTimerStatus;  // 타이머 영역
static HWND          g_hWordStatus;   // 제시어 영역
static int           g_gameScore;     // 게임 점수
extern _TCHAR* messageQueue[10];      // 메시지 큐(도중에 들어온 클라이언트에게 이전 채팅 내용 표시
static HWND          g_hDrawDlg;
static BOOL          isMessageQueue = FALSE;
static MESSAGEQUEUE g_msgQueue;

// 게임 관련 변수
static int roundNum = 0;   //진행한 문제 개수. 제시어 배열의 인덱스 역할도 한다.
static int countdown = 30;
static const _TCHAR* quizWord[4] = { _T("apple"), _T("바나나"), _T("포도"),_T("오렌지") };   // 제시어 배열
static BOOL isGameOver = FALSE;
static BOOL isOwner = FALSE;  // 문제를 내는 클라이언트일 경우 isOwner는 TRUE이다. 문제를 맞추는 사람인 경우 FALSE.


static char NICKNAME_CHAR[256];

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
#define ID_CHANNEL_A_BUTTON		2500	// 채널 A 버튼 ( TCP)
#define ID_CHANNEL_B_BUTTON		2501	// 채널 B 버튼 ( UDP1)
#define ID_CHANNEL_RANDOM_BUTTON 2502	// 랜덤 접속 버튼 (UDP2)
#define ID_NOTICE_BUTTON		2506	// 공지 전송 버튼

// 홈 관련 전역 변수
extern _TCHAR		ID_NOTICE_TEXT[256];	// 공지사항 내용
extern _TCHAR		ID_NOTICE_INPUT_TEXT[256];	// 공지사항 입력 내용

// 홈 채널 관련 전역 변수 및 상수
#define CHANNEL_TCP				0		// TCP 채널
#define CHANNEL_UDP1			1		// UDP 채널 1
#define CHANNEL_UDP2			2		// UDP 채널 2
extern int			channel;				//채널. (0 : top, 1(udp1) 또는 2(udp2)가 있다.) 

// 홈 공지사항 사용자 정의 상수 (2600~ )
#define ID_NOTICE_INPUT			2600	// 홈 공지사항 입력 input
#define ID_OK_BUTTON			2601	// 확인 버튼
#define ID_CANCLE_BUTTON		2602	// 취소 버튼

// 홈 공지사항 비밀번호 사용자 정의 상수 (2700 ~)
#define ID_PASSWORD_INPUT		2700	// 비밀번호 입력 input

// 윈도우 관련 전역 변수
static HWND			hwndLogin;			// 로그인 윈도우 (2000..)
static HWND			hwndHome;			// 홈 화면 윈도우 (2500..)
static HWND			hwndHome_Pass;		// 공지사항 비밀번호 윈도우
static HWND			hwndHome_Notice;	// 공지사항 입력 윈도우

// ======= 정호 =========
#define TYPE_DRAWELLIPSE 4000			// 메시지 타입 : 타원 그리기
#define WM_DRAWELLIPSE (WM_USER+3)		// 타원 그리기 윈도우 메시지
#define WM_ERASEALITTLE (WM_USER+4)		// 특정 부분 조금 지우기 윈도우 메시지
#define WM_DRAWRECTANGLE (WM_USER+5)	// 메시지 타입 : 사각형 그리기
#define WM_DRAWTRIAGNGLE (WM_USER+6)	// 메시지 타입 : 삼각형 그리기

// 그리기 모드 종류
#define MODE_ERASE 4500
#define MODE_LINE 4501
#define MODE_ELLIPSE 4502
#define MODE_RECTANGLE 4503
#define MODE_TRIANGLE 4504

// 현재 그리기 모드 전역 변수
static int g_currentSelectFigureMode = MODE_LINE;

// 현재 서버로부터 받은세부 그리기 정보 전역 변수
static DRAW_DETAIL_INFORMATION g_serverDrawDetailInformation;

// 현재 클라이언트가 선택한 그리기 정보 전역 변수
static DRAW_DETAIL_INFORMATION g_clientDrawDetailInformation;

// 지우개 모드 선택하기 전 클라이언트가 마지막으로 선택한 색깔
static int g_lastSelectColor = RGB(255, 0, 0);

// 이전 모드가 "지우개" 모드인지 확인하고 한 번만 실행하도록 설정
static bool g_isBeforeModeErase = false;

static HWND g_hFigureSelect; // 그릴 도형 선택하는 부분

static DRAWELLIPSE_MSG g_drawellipsemsg; // 타원 그리기 메시지

static SOCKADDR_IN serveraddr; // 서버 주소
//