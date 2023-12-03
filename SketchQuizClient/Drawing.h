#pragma once

// Ÿ�� ����
// ======================= ��ȣ =======================

// Ÿ��
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

// �簢��
typedef struct _DRAWRECTANGLE_MSG
{
	int type;
	int width;
	int color;
	int startX, startY;
	int endX, endY;
	char dummy[SIZE_TOT - 7 * sizeof(int)];
} DRAWRECTANGLE_MSG;

// �׸� ���� ������ ���� ����ü
typedef struct _DRAW_DETAIL_INFORMATION
{
	int width = 1;	// ����
	int color = RGB(255,0,0);	// ����
} DRAW_DETAIL_INFORMATION;

// ��� ����
// ======================= ���� =======================
// ��ȭ ���ڸ� ����� ǥ���ϴ� �Լ�
void CreateAndShowDialog(HWND hWnd);
void SelectPenColor(DRAW_DETAIL_INFORMATION* g_drawDetailInformation);
void AddLineWidthOption(HWND hDlg);
void SelectLineWidth(HWND hDlg, DRAW_DETAIL_INFORMATION* g_drawDetailInformation);
char* GetRandomUserID(char* userIDs[], int numUsers);




// ======================= ��ȣ =======================

// ���� �ɼ� ���� �׸� �߰�
void AddFigureOption(HWND hDlg);

// �׸��� �ɼ� ����
void SelectFigureOption(HWND hDlg, int &g_currentSelectFigureOption);

// Ÿ�� �׸��� ���� ����
void DrawEllipseProcess(HWND hWnd, HDC& hDCMem, WPARAM wParam, LPARAM lParam, DRAW_DETAIL_INFORMATION drawDetailInformation);

// Ÿ���� Ư�� HDC�� �׸�
void DrawEllipseInHDC(HDC tHDC, WPARAM wParam, LPARAM lParam);

// �� �׸��� ���� ����
void DrawLineProcess(HWND hWnd, HDC& hDCMem, WPARAM wParam, LPARAM lParam, DRAW_DETAIL_INFORMATION drawDetailInformation);

// ���� Ư�� HDC�� �׸�
void DrawLineInHDC(HDC tHDC, WPARAM wParam, LPARAM lParam);