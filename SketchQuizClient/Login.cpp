#include "stdafx.h"



// �α��� â�� ����� ǥ���ϴ� �Լ�
void CreateAndShowWindow_Login(HWND hWnd)
{
	// �α��� â ����
	hwndLogin = CreateWindow(_T("LoginWindowClass"), _T("�α��� â"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, NULL, NULL, g_hInstance, NULL);
	ShowWindow(hwndLogin, SW_SHOW);
}
