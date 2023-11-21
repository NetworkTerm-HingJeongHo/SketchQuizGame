#pragma once

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
/*Drawing.h에 빼뒀음*/

// 메인 윈도우(첫화면) 프로시저
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// 에디트 컨트롤 출력 함수
void DisplayText(const char *fmt, ...);
