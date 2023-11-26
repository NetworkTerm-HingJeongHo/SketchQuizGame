#include "stdafx.h"

// 대화 상자를 만들고 표시하는 함수
void CreateAndShowDialog(HWND hWnd)
{
	g_hDialog = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc);
	ShowWindow(g_hDialog, SW_SHOW);
	g_bDialogVisible = true;
}

// ======================= 지윤 =======================






// ======================= 정호 =======================

void DrawEllipseProcess(HWND hWnd, HDC& hDCMem, WPARAM wParam, LPARAM lParam, int startX, int startY)
{
	HDC hDC = GetDC(hWnd);
	HPEN hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));

	// 윈도우 DC에 타원 출력
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

	int endX = LOWORD(lParam);
	int endY = HIWORD(lParam);

	int centerX = (startX + endX) / 2;
	int centerY = (startY + endY) / 2;

	int ellipseAxisX = centerX - startX;
	int ellipseAxisY = centerY - startY;
	double angle;
	int newX, newY;

	int oldX = endX;
	int oldY = centerY;
	for (int i = 0; i <= 360; i++)
	{
		angle = 2 * 3.1416 * i / 360;
		newX = centerX + ellipseAxisX * cos(angle);
		newY = centerY + ellipseAxisY * sin(angle);

		MoveToEx(hDC, oldX, oldY, NULL);
		LineTo(hDC, newX, newY);

		oldX = newX;
		oldY = newY;
	}

	SelectObject(hDC, hOldPen);
	hOldPen = (HPEN)SelectObject(hDCMem, hPen);

	for (int i = 0; i <= 360; i++)
	{
		angle = 2 * 3.1416 * i / 360;
		newX = centerX + ellipseAxisX * cos(angle);
		newY = centerY + ellipseAxisY * sin(angle);

		MoveToEx(hDCMem, oldX, oldY, NULL);
		LineTo(hDCMem, newX, newY);

		oldX = newX;
		oldY = newY;
	}

	SelectObject(hDCMem, hOldPen);

	// 화면 출력용 DC와 Pen 핸들 해제
	DeleteObject(hPen);
	ReleaseDC(hWnd, hDC);
}