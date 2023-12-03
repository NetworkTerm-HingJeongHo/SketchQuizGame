#pragma once

#define SERVERIP4_CHAR_UDP1 "235.7.8.18" // �׷� 1
#define SERVERIP4_CHAR_UDP2 "235.7.8.19" // �׷� 1
#define SERVERPORT 9000
#define BUFSIZE    256
// ==== ��ȣ ====
#define WM_SOCKET (WM_USER+1)

// ���� ���� ������ ���� ����ü�� ����
typedef struct _SOCKETINFO
{
	SOCKET sock;
	char   buf[BUFSIZE + 1];
	int    recvbytes;

	// ===== ���� ======
	_TCHAR id_nickname[BUFSIZE]; // ����� �г���
	int    score = 0;       // ���� �÷��� ����
} SOCKETINFO;

// ======= ���� ======= 
typedef struct _MESSAGEQUEUE {
	char queue[BUFSIZE][100] = { NULL };         // �޽��� ���� ť: ���� ��ȭ���� ǥ��. �� ���� ���� ������ �޽������� ��������.
	int head = 0;                 // ���� ť �ε���
	int tail = 0;
} MESSAGEQUEUE;

// ====================


// ========== ���� ==========
// ���̾�α� ���ν���
INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
// ==========================

// ���� ���� ���� �Լ�
bool AddSocketInfoTCP(SOCKET sock);
bool AddSocketInfoUDP(SOCKADDR_IN addr);
void RemoveSocketInfo(SOCKET sock);
void addMessage(char* message);
SOCKETINFO* GetSocketInfo(SOCKET sock);

// ������ �޽��� ó�� �Լ�
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);