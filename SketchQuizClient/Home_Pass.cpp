#include "stdafx.h"

// �н����� �Է� â�� ����� ǥ���ϴ� �Լ�
void CreateAndShowWindow_Home_Pass(HWND hWnd)
{
	// �α��� â ����
	hwndHome_Pass = CreateWindow(_T("Home_PassWindowClass"), _T("�н����� �Է� â"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, g_hInstance, NULL);
	ShowWindow(hwndHome_Pass, SW_SHOW);
}
