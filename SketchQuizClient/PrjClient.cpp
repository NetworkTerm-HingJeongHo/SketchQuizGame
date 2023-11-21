#include "stdafx.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 이벤트 생성(각각 신호, 비신호 상태)
	g_hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if (g_hReadEvent == NULL) return 1;
	g_hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (g_hWriteEvent == NULL) return 1;

	// 전역 변수 초기화(일부)
	g_chatmsg.type = TYPE_CHAT;
	g_drawlinemsg.type = TYPE_DRAWLINE;
	g_drawlinemsg.color = RGB(255, 0, 0);
	g_erasepicmsg.type = TYPE_ERASEPIC;

	g_hInstance = hInstance;

	// 메인 윈도우(첫 화면) 생성
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, MainWndProc, 0, 0, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("MainWindow"), NULL };
	RegisterClassEx(&wcex);
	g_hMainWindow = CreateWindow(_T("MainWindow"), _T("Main Window"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 200, NULL, NULL, hInstance, NULL);

	ShowWindow(g_hMainWindow, nCmdShow);
	UpdateWindow(g_hMainWindow);

	// 메시지 루프
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!IsDialogMessage(g_hDialog, &msg)) // 대화 상자 메시지 처리
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// 이벤트 객체 제거
	CloseHandle(g_hReadEvent);
	CloseHandle(g_hWriteEvent);
	// 윈속 종료
	WSACleanup();
	return 0;
}

// 메인 윈도우 프로시저
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		// '그림판' 버튼 생성
		CreateWindow(_T("BUTTON"), _T("그림판"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 10, 10, 100, 30, hWnd, (HMENU)1, g_hInstance, NULL);

		// '로그인' 버튼 생성
		CreateWindow(_T("BUTTON"), _T("로그인"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 120, 10, 100, 30, hWnd, (HMENU)2, g_hInstance, NULL);

		// '랭킹' 버튼 생성 (세 번째로 위치)
		CreateWindow(_T("BUTTON"), _T("랭킹"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 230, 10, 100, 30, hWnd, (HMENU)4, g_hInstance, NULL);

		// '메인' 버튼 생성 (네 번째로 위치)
		CreateWindow(_T("BUTTON"), _T("메인"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 340, 10, 100, 30, hWnd, (HMENU)3, g_hInstance, NULL);



		break;
	}
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == 1) // '그림판' 버튼 클릭
		{
			if (!g_bDialogVisible) // 대화 상자가 현재 보이지 않는 경우
			{
				// 대화 상자를 만들고 표시하는 함수 호출
				CreateAndShowDialog(hWnd);
			}
			else
			{
				// 대화 상자가 이미 보이는 경우, 대화 상자를 활성화합니다.
				SetForegroundWindow(g_hDialog);
			}
			// 빈 윈도우창 숨기기
			ShowWindow(hWnd, SW_HIDE);
		}
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 에디트 컨트롤 출력 함수
void DisplayText(const char *fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);
	char cbuf[1024];
	vsprintf(cbuf, fmt, arg);
	va_end(arg);

	int nLength = GetWindowTextLength(g_hEditStatus);
	SendMessage(g_hEditStatus, EM_SETSEL, nLength, nLength);
	SendMessageA(g_hEditStatus, EM_REPLACESEL, FALSE, (LPARAM)cbuf);
}
