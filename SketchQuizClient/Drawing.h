#pragma once

// 대화 상자를 만들고 표시하는 함수
void CreateAndShowDialog(HWND hWnd);

// 타입 정의
// ======================= 정호 =======================

// sizeof(DRAWELLIPSE_MSG) == 256
typedef struct _DRAWELLIPSE_MSG
{
	int  type;
	int	 width;
	int  color;
	int  x0, y0;
	int  x1, y1;
	char dummy[SIZE_TOT - 7 * sizeof(int)];
} DRAWELLIPSE_MSG;

// 그림 세부 정보에 대한 구조체
typedef struct _DRAW_DETAIL_INFORMATION
{
	int width = 1;	// 굵기
	int color = RGB(255,0,0);	// 색상
} DRAW_DETAIL_INFORMATION;

// 기능 정의
// ======================= 지윤 =======================
void SelectPenColor(DRAW_DETAIL_INFORMATION* g_drawDetailInformation);
void AddLineWidthOption(HWND hDlg);
void SelectLineWidth(HWND hDlg, DRAW_DETAIL_INFORMATION* g_drawDetailInformation);





// ======================= 정호 =======================

// 도형 옵션 선택 항목 추가
void AddFigureOption(HWND hDlg);

// 그리기 옵션 선택
void SelectFigureOption(HWND hDlg, int &g_currentSelectFigureOption);

// 타원 그리기 과정 실행
void DrawEllipseProcess(HWND hWnd, HDC& hDCMem, WPARAM wParam, LPARAM lParam, DRAW_DETAIL_INFORMATION drawDetailInformation);

// 타원을 특정 HDC에 그림
void DrawEllipseInHDC(HDC tHDC, WPARAM wParam, LPARAM lParam);