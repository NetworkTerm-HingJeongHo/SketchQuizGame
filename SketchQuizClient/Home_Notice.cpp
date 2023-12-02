#include "stdafx.h"

// Ȩ â�� ����� ǥ���ϴ� �Լ�
void CreateAndShowWindow_Home_Notice(HWND hWnd)
{
	// Ȩ â ����
	hwndHome_Notice = CreateWindow(_T("Home_NoticeWindowClass"), _T("�������� �Է�â"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, g_hInstance, NULL);

	if (hwndHome_Notice == NULL)
	{
		MessageBox(NULL, L"������ ���� ����!", L"����", MB_ICONERROR);
	}
	ShowWindow(hwndHome_Notice, SW_SHOW);
}


// ���������� UDP ��ε�ĳ��Ʈ�� ������ �Լ�




//#define REMOTEPORT 9000

int BoradcaseSendToNotice(_TCHAR* input_notice_result)
{
	MessageBox(hwndHome_Notice, input_notice_result, _T("�������� ���� ���� �߰����� Ȯ��"), MB_OK);
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// ��ε�ĳ���� Ȱ��ȭ
	BOOL bEnable = TRUE;
	retval = setsockopt(sock, SOL_SOCKET, SO_BROADCAST,
		(char*)&bEnable, sizeof(bEnable));
	if (retval == SOCKET_ERROR) err_quit("setsockopt()");

	// ���� �ּ� ����ü �ʱ�ȭ
	SOCKADDR_IN remoteaddr; //����
	ZeroMemory(&remoteaddr, sizeof(remoteaddr));
	remoteaddr.sin_family = AF_INET;
	remoteaddr.sin_addr.s_addr = inet_addr(SERVERIP4_CHAR); //������ ip
	remoteaddr.sin_port = htons(SERVERPORT); // �Ʊ� �����ص� ���� port��ȣ

	// ������ ��ſ� ����� ����
	char buf[257];
	int len;

	// ��ε�ĳ��Ʈ ������ ������

	// ������ �Է�
										//�ٲ� ��� - ������ ��
	WideCharToMultiByte(CP_ACP, 0, input_notice_result, 256, buf, 256, NULL, NULL); //_TCHAR �� ���ڿ��� char* �� ���ڿ��� ����
	// ������ ������
	retval = sendto(sock, buf, strlen(buf), 0,
		(SOCKADDR*)&remoteaddr, sizeof(remoteaddr)); //&remoteaddr : ������ ������ �ּҸ� ������ �־�����Ѵ�.
	if (retval == SOCKET_ERROR) {
		err_display("sendto()");
	}
	//printf("[UDP] %d����Ʈ�� ���½��ϴ�.\n", retval);


	// closesocket()
	closesocket(sock);

	// ���� ����
	WSACleanup();

	return 0;

}

