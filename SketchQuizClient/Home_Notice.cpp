#include "stdafx.h"

// 홈 창을 만들고 표시하는 함수
void CreateAndShowWindow_Home_Notice(HWND hWnd)
{
	// 홈 창 생성
	hwndHome_Notice = CreateWindow(_T("Home_NoticeWindowClass"), _T("공지사항 입력창"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, g_hInstance, NULL);

	if (hwndHome_Notice == NULL)
	{
		MessageBox(NULL, L"윈도우 생성 실패!", L"에러", MB_ICONERROR);
	}
	ShowWindow(hwndHome_Notice, SW_SHOW);
	g_bDialogVisible = true;
}


