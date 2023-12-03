#pragma once

/*���� ���*/
#include "Winsock.h"
#include "PrjClient.h"

// ====== ���� ======
#include "Drawing.h"

//* ���� ���� ��� *//
#include <stdlib.h>
#include "Home.h"
#include "Login.h"
#include "Home_Notice.h"
#include "Home_Pass.h"

// ====== ��ȣ ======
#include "math.h"
#include "socket.h"

// ====== ���� ======
#include "Ranking.h"
#include "Chatting.h"
#include "Game.h"
#include <time.h>

/*���ҽ�*/
#include "resource.h" // �׸��� ���̾�α�â

/*�ĺ���*/
#define SERVERIP4_CHAR_UDP1 "239.0.0.1" // UDP ����1 (����)
#define SERVERIP4_CHAR_UDP2 "239.0.0.2" // UDP ����2 (����)
#define SERVERIP4_CHAR   "127.0.0.1" //��ε�ĳ��Ʈ ��� �ּ� �� ���� ip char ���� (����)
#define SERVERIP4  _T("127.0.0.1")
#define SERVERIP6  _T("::1")
#define SERVERPORT  9000

/* ������ ���� ���� ���� */
static HINSTANCE     g_hInstance;     // ���α׷� �ν��Ͻ� �ڵ�
static HWND          g_hBtnSendFile;  // [���� ����] ��ư
static HWND          g_hBtnSendMsg;   // [�޽��� ����] ��ư
static HWND          g_hEditStatus;   // ���� �޽��� ��� ����
static HWND          g_hBtnErasePic;  // [�׸� �����] ��ư
static HWND          g_hDrawWnd;      // �׸��� �׸� ������

// ============= ���� =============
static HWND			 g_hMainWindow;   // ó�� ������â �ڵ�
static HWND			 g_hDialog;       // �׸��� ���̾�α� �ڵ�
static HWND			 g_hBtnPenColor;  // [���� ����] ��ư
static HWND			 g_hLineWidth;    // �� ���� �޺��ڽ�

// ============= ���� =============
// ä�� ���� ����
#define TYPE_NOTY 5000  // ����, ����, �� ���� �˸� �޽��� ���

static HWND          g_hRankDlg;      // ��ŷ ���̾�α� �ڵ�
static HWND          g_hChattingDlg;  // ä�� ���̾�α� �ڵ�
static HWND          g_hTimerStatus;  // Ÿ�̸� ����
static HWND          g_hWordStatus;   // ���þ� ����
static int           g_gameScore;     // ���� ����
extern _TCHAR* messageQueue[10];      // �޽��� ť(���߿� ���� Ŭ���̾�Ʈ���� ���� ä�� ���� ǥ��
static HWND          g_hDrawDlg;
static BOOL          isMessageQueue=FALSE;  // �޽��� ť ����ü�� ���۹޴� ��츦 ������ �� �ֵ��� �ϴ� flag
static MESSAGEQUEUE g_msgQueue;

// ���� ���� ����
static int roundNum = 0;   //������ ���� ����. ���þ� �迭�� �ε��� ���ҵ� �Ѵ�.
static int countdown = 30;
static const _TCHAR* quizWord[4] = { _T("apple"), _T("�ٳ���"), _T("����"),_T("������") };   // ���þ� �迭
static BOOL isGameOver = FALSE;
static BOOL isOwner = FALSE;  // ������ ���� Ŭ���̾�Ʈ�� ��� isOwner�� TRUE�̴�. ������ ���ߴ� ����� ��� FALSE.

static char NICKNAME_CHAR[256];
// =====================================

/* ��� ���� ���� ���� */
static volatile bool g_isIPv6;        // IPv4 or IPv6 �ּ�
static char          g_ipaddr[64];    // ���� IP �ּ�(���ڿ�)
static int           g_port;          // ���� ��Ʈ ��ȣ
static volatile bool g_isUDP;         // TCP or UDP ��������
static HANDLE        g_hClientThread; // ������ �ڵ�
static volatile bool g_bCommStarted;  // ��� ���� ����
static SOCKET        g_sock;          // Ŭ���̾�Ʈ ����
static HANDLE        g_hReadEvent;    // �̺�Ʈ �ڵ�(1)
static HANDLE        g_hWriteEvent;   // �̺�Ʈ �ڵ�(2)

/* �޽��� ���� ���� ���� */
static CHAT_MSG      g_chatmsg;       // ä�� �޽���
static DRAWLINE_MSG  g_drawlinemsg;   // �� �׸��� �޽���
static int           g_drawcolor;     // �� �׸��� ����
static ERASEPIC_MSG  g_erasepicmsg;   // �׸� ����� �޽���

// ============= ���� =============
static int			 g_lineWidth;     // �� �׸��� ����


/* ���� (2000������) */
// �α��� ����� ���� ���
#define ID_LOGIN_BUTTON			2000	// �α��� ��ư
#define ID_ID_INPUT				2001	//�α��� input
#define ID_DUPLICATION_BUTTON	2002	// �ߺ� Ȯ�� ��ư
#define ID_BACKHOME_BUTTON		2003	// ���ư��� ��ư

// �α��� ���� ���� ����
extern _TCHAR		ID_NICKNAME[256];	// ���� ����� ���̵� ��������

// Ȩ ����� ���� ���
#define ID_CHANNEL_A_BUTTON		2500	// ä�� A ��ư ( TCP)
#define ID_CHANNEL_B_BUTTON		2501	// ä�� B ��ư ( UDP1)
#define ID_CHANNEL_RANDOM_BUTTON 2502	// ���� ���� ��ư (UDP2)
#define ID_NOTICE_BUTTON		2506	// ���� ���� ��ư

// Ȩ ���� ���� ����
extern _TCHAR		ID_NOTICE_TEXT[256];	// �������� ����
extern _TCHAR		ID_NOTICE_INPUT_TEXT[256];	// �������� �Է� ����

// Ȩ ä�� ���� ���� ���� �� ���
#define CHANNEL_TCP				0		// TCP ä��
#define CHANNEL_UDP1			1		// UDP ä�� 1
#define CHANNEL_UDP2			2		// UDP ä�� 2
extern int			channel;				//ä��. (0 : top, 1(udp1) �Ǵ� 2(udp2)�� �ִ�.) 

// Ȩ �������� ����� ���� ��� (2600~ )
#define ID_NOTICE_INPUT			2600	// Ȩ �������� �Է� input
#define ID_OK_BUTTON			2601	// Ȯ�� ��ư
#define ID_CANCLE_BUTTON		2602	// ��� ��ư

// Ȩ �������� ��й�ȣ ����� ���� ��� (2700 ~)
#define ID_PASSWORD_INPUT		2700	// ��й�ȣ �Է� input

// ������ ���� ���� ����
static HWND			hwndLogin;			// �α��� ������ (2000..)
static HWND			hwndHome;			// Ȩ ȭ�� ������ (2500..)
static HWND			hwndHome_Pass;		// �������� ��й�ȣ ������
static HWND			hwndHome_Notice;	// �������� �Է� ������

// ======= ��ȣ =========
#define TYPE_DRAWELLIPSE 4000			// �޽��� Ÿ�� : Ÿ�� �׸���
#define WM_DRAWELLIPSE (WM_USER+3)		// Ÿ�� �׸��� ������ �޽���
#define WM_ERASEALITTLE (WM_USER+4)		// Ư�� �κ� ���� ����� ������ �޽���
#define WM_DRAWRECTANGLE (WM_USER+5)	// �޽��� Ÿ�� : �簢�� �׸���
#define WM_DRAWTRIAGNGLE (WM_USER+6)	// �޽��� Ÿ�� : �ﰢ�� �׸���

// �׸��� ��� ����
#define MODE_ERASE 4500
#define MODE_LINE 4501
#define MODE_ELLIPSE 4502
#define MODE_RECTANGLE 4503
#define MODE_TRIANGLE 4504

// ���� �׸��� ��� ���� ����
static int g_currentSelectFigureMode = MODE_LINE;

// ���� �����κ��� �������� �׸��� ���� ���� ����
static DRAW_DETAIL_INFORMATION g_serverDrawDetailInformation;

// ���� Ŭ���̾�Ʈ�� ������ �׸��� ���� ���� ����
static DRAW_DETAIL_INFORMATION g_clientDrawDetailInformation;

// ���찳 ��� �����ϱ� �� Ŭ���̾�Ʈ�� ���������� ������ ����
static int g_lastSelectColor = RGB(255, 0, 0);

// ���� ��尡 "���찳" ������� Ȯ���ϰ� �� ���� �����ϵ��� ����
static bool g_isBeforeModeErase = false;

static HWND g_hFigureSelect; // �׸� ���� �����ϴ� �κ�

static DRAWELLIPSE_MSG g_drawellipsemsg; // Ÿ�� �׸��� �޽���

static SOCKADDR_IN serveraddr; // ���� �ּ�
//