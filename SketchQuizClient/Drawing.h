#pragma once

// 대화 상자를 만들고 표시하는 함수
void CreateAndShowDialog(HWND hWnd);


// ======================= 지윤 =======================
void SelectPenColor(DRAWLINE_MSG* g_drawlinemsg);
void AddLineWidthOption(HWND hDlg);
void SelectLineWidth(HWND hDlg, DRAWLINE_MSG* g_drawlinemsg);





// ======================= 정호 =======================

// sizeof(DRAWELLIPSE_MSG) == 256
typedef struct _DRAWELLIPSE_MSG
{
	int  type;
	int  color;
	int  x0, y0;
	int  x1, y1;
	char dummy[SIZE_TOT - 6 * sizeof(int)];
} DRAWELLIPSE_MSG;

// 도형 옵션 선택 항목 추가
void AddFigureOption(HWND hDlg);

// 그리기 옵션 선택
void SelectFigureOption(HWND hDlg, int &g_currentSelectFigureOption);

// 타원 그리기
void DrawEllipseProcess(HWND hWnd, HDC& hDCMem, WPARAM wParam, LPARAM lParam, int startX, int startY, HPEN& hPen);