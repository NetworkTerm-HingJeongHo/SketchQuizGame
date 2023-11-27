#include "stdafx.h"

// 대화 상자를 만들고 표시하는 함수
void CreateAndShowDialog(HWND hWnd)
{
	g_hDialog = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc);
	ShowWindow(g_hDialog, SW_SHOW);
	g_bDialogVisible = true;
}

// ======================= 지윤 =======================
void SelectPenColor(DRAWLINE_MSG *g_drawlinemsg) {
	// 색상 대화 상자 열기
	CHOOSECOLOR cc = { sizeof(CHOOSECOLOR) };
	static COLORREF customColors[16] = { 0 }; // 사용자 정의 색상
	cc.hwndOwner = g_hDrawWnd;
	cc.lpCustColors = customColors;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	cc.rgbResult = g_drawlinemsg->color;
	if (ChooseColor(&cc)) {
		g_drawlinemsg->color = cc.rgbResult;
	}
}

void AddLineWidthOption(HWND hDlg) {
	// 항목 추가
	SendDlgItemMessage(hDlg, IDC_LINEWIDTH, CB_ADDSTRING, 0, (LPARAM)_T("얇게"));
	SendDlgItemMessage(hDlg, IDC_LINEWIDTH, CB_ADDSTRING, 0, (LPARAM)_T("보통"));
	SendDlgItemMessage(hDlg, IDC_LINEWIDTH, CB_ADDSTRING, 0, (LPARAM)_T("굵게"));
	SendDlgItemMessage(hDlg, IDC_LINEWIDTH, CB_SETCURSEL, 0, 0); // 초기 선택은 "얇게"
}

void SelectLineWidth(HWND hDlg, DRAWLINE_MSG* g_drawlinemsg) {
	// Combo Box의 선택이 변경되면 이벤트 처리
	int selectedIndex = SendDlgItemMessage(hDlg, IDC_LINEWIDTH, CB_GETCURSEL, 0, 0);
	switch (selectedIndex)
	{
	case 0: // "얇게" 선택
		// 펜의 굵기를 1px로 설정
		g_drawlinemsg->width = 1;
		break;
	case 1: // "보통" 선택
		// 펜의 굵기를 3px로 설정
		g_drawlinemsg->width = 3;
		break;
	case 2: // "굵게" 선택
		// 펜의 굵기를 5px로 설정
		g_drawlinemsg->width = 5;
		break;
	}
}




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