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

// 도형 옵션 선택 항목 추가
void AddFigureOption(HWND hDlg)
{
	SendDlgItemMessage(hDlg, IDC_FIGURE, CB_ADDSTRING, 0, (LPARAM)_T("지우개"));
	SendDlgItemMessage(hDlg, IDC_FIGURE, CB_ADDSTRING, 0, (LPARAM)_T("선"));
	SendDlgItemMessage(hDlg, IDC_FIGURE, CB_ADDSTRING, 0, (LPARAM)_T("타원"));
	SendDlgItemMessage(hDlg, IDC_FIGURE, CB_ADDSTRING, 0, (LPARAM)_T("사각형"));

	// 초기 도형 옵션은 "선"으로 설정 
	SendDlgItemMessage(hDlg, IDC_FIGURE, CB_SETCURSEL, 1, 0);
}

// 그리기 옵션 선택
void SelectFigureOption(HWND hDlg, int &g_currentSelectFigureOption)
{
	// ComboBox의 선택이 변경되면, 이벤트 처리
	int selectedIndex = SendDlgItemMessage(hDlg, IDC_FIGURE, CB_GETCURSEL, 0, 0);
	switch (selectedIndex)
	{
	// "지우개" 모드 선택
	case 0:
		g_currentSelectFigureOption = MODE_ERASE;
		break;
	// "선" 모드 선택
	case 1: 
		g_currentSelectFigureOption = MODE_LINE;
		break;
	// "타원" 모드 선택
	case 2: 
		g_currentSelectFigureOption = MODE_ELLIPSE;
		break;
	// "사각형" 모드 선택
	case 3:
		g_currentSelectFigureOption = MODE_RECTANGLE;
		break;
	// "삼각형" 모드 선택
	case 4:
		g_currentSelectFigureOption = MODE_TRIANGLE;
		break;
	}
}

// 타원 그리기
void DrawEllipseProcess(HWND hWnd, HDC& hDCMem, WPARAM wParam, LPARAM lParam, int startX, int startY, HPEN& hPen)
{
	HDC hDC = GetDC(hWnd);

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