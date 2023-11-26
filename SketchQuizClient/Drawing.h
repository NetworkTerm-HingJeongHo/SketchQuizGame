#pragma once

// 대화 상자를 만들고 표시하는 함수
void CreateAndShowDialog(HWND hWnd);


// ======================= 지윤 =======================






// ======================= 정호 =======================
#define TYPE_DRAWELLIPSE 4000			// 메시지 타입 : 타원 그리기
#define WM_DRAWELLIPSE (WM_USER+3)		// 타원 그리기 윈도우 메시지

// sizeof(DRAWELLIPSE_MSG) == 256
typedef struct _DRAWELLIPSE_MSG
{
	int  type;
	int  color;
	int  x0, y0;
	int  x1, y1;
	char dummy[SIZE_TOT - 6 * sizeof(int)];
} DRAWELLIPSE_MSG;

void DrawEllipseProcess(HWND hWnd, HDC& hDCMem, WPARAM wParam, LPARAM lParam, int startX, int startY);