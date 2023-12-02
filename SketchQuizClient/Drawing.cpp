#include "stdafx.h"

// ======================= ���� =======================
// �׸���â ���̾�α׸� ����� ǥ���ϴ� �Լ�
void CreateAndShowDialog(HWND hWnd)
{
	g_hDialog = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc);
	ShowWindow(g_hDialog, SW_SHOW);
}

void SelectPenColor(DRAW_DETAIL_INFORMATION* g_drawDetailInformation) {
	// ���� ��ȭ ���� ����
	CHOOSECOLOR cc = { sizeof(CHOOSECOLOR) };
	static COLORREF customColors[16] = { 0 }; // ����� ���� ����
	cc.hwndOwner = g_hDrawWnd;
	cc.lpCustColors = customColors;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	cc.rgbResult = g_drawDetailInformation->color;
	if (ChooseColor(&cc)) {
		g_drawDetailInformation->color = cc.rgbResult;
	}
}

void AddLineWidthOption(HWND hDlg) {
	// �׸� �߰�
	SendDlgItemMessage(hDlg, IDC_LINEWIDTH, CB_ADDSTRING, 0, (LPARAM)_T("���"));
	SendDlgItemMessage(hDlg, IDC_LINEWIDTH, CB_ADDSTRING, 0, (LPARAM)_T("����"));
	SendDlgItemMessage(hDlg, IDC_LINEWIDTH, CB_ADDSTRING, 0, (LPARAM)_T("����"));
	SendDlgItemMessage(hDlg, IDC_LINEWIDTH, CB_SETCURSEL, 0, 0); // �ʱ� ������ "���"
}

void SelectLineWidth(HWND hDlg, DRAW_DETAIL_INFORMATION* g_drawDetailInformation) {
	// Combo Box�� ������ ����Ǹ� �̺�Ʈ ó��
	int selectedIndex = SendDlgItemMessage(hDlg, IDC_LINEWIDTH, CB_GETCURSEL, 0, 0);
	switch (selectedIndex)
	{
	case 0: // "���" ����
		// ���� ���⸦ 1px�� ����
		g_drawDetailInformation->width = 1;
		break;
	case 1: // "����" ����
		// ���� ���⸦ 3px�� ����
		g_drawDetailInformation->width = 3;
		break;
	case 2: // "����" ����
		// ���� ���⸦ 5px�� ����
		g_drawDetailInformation->width = 5;
		break;
	}
}

// �����ϰ� ����� ���̵� �����ϴ� �Լ�
char* GetRandomUserID(char* userIDs[], int numUsers) {
	if (numUsers <= 0) {
		return NULL; // ��ȿ���� ���� ���� ó��
	}

	// �õ尪 ����
	srand(time(NULL));

	// ���� �ε��� ����
	int randomIndex = rand() % numUsers;

	// �������� ���õ� ����� ���̵� ��ȯ
	return userIDs[randomIndex];
}


// ======================= ��ȣ =======================

// ���� �ɼ� ���� �׸� �߰�
void AddFigureOption(HWND hDlg)
{
	SendDlgItemMessage(hDlg, IDC_FIGURE, CB_ADDSTRING, 0, (LPARAM)_T("���찳"));
	SendDlgItemMessage(hDlg, IDC_FIGURE, CB_ADDSTRING, 0, (LPARAM)_T("��"));
	SendDlgItemMessage(hDlg, IDC_FIGURE, CB_ADDSTRING, 0, (LPARAM)_T("Ÿ��"));
	SendDlgItemMessage(hDlg, IDC_FIGURE, CB_ADDSTRING, 0, (LPARAM)_T("�簢��"));
	SendDlgItemMessage(hDlg, IDC_FIGURE, CB_ADDSTRING, 0, (LPARAM)_T("�ﰢ��"));

	// �ʱ� ���� �ɼ��� "��"���� ���� 
	SendDlgItemMessage(hDlg, IDC_FIGURE, CB_SETCURSEL, 1, 0);
}

// �׸��� �ɼ� ����
void SelectFigureOption(HWND hDlg, int &g_currentSelectFigureOption)
{
	// ComboBox�� ������ ����Ǹ�, �̺�Ʈ ó��
	int selectedIndex = SendDlgItemMessage(hDlg, IDC_FIGURE, CB_GETCURSEL, 0, 0);
	switch (selectedIndex)
	{
	// "���찳" ��� ����
	case 0:
		g_currentSelectFigureOption = MODE_ERASE;
		break;
	// "��" ��� ����
	case 1: 
		g_currentSelectFigureOption = MODE_LINE;
		break;
	// "Ÿ��" ��� ����
	case 2: 
		g_currentSelectFigureOption = MODE_ELLIPSE;
		break;
	// "�簢��" ��� ����
	case 3:
		g_currentSelectFigureOption = MODE_RECTANGLE;
		break;
	// "�ﰢ��" ��� ����
	case 4:
		g_currentSelectFigureOption = MODE_TRIANGLE;
		break;
	}
}

// Ÿ�� �׸��� ���� ����
void DrawEllipseProcess(HWND hWnd, HDC& hDCMem, WPARAM wParam, LPARAM lParam, DRAW_DETAIL_INFORMATION drawDetailInformation)
{
	HDC hDC = GetDC(hWnd);
	HPEN hPen = CreatePen(PS_SOLID, drawDetailInformation.width, drawDetailInformation.color);
	// ������ DC�� Ÿ�� ���
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);

	// ������ ȭ�鿡 Ÿ���� 1���� �׸���
	DrawEllipseInHDC(hDC, wParam, lParam);

	SelectObject(hDC, hOldPen);
	hOldPen = (HPEN)SelectObject(hDCMem, hPen);

	// ��� ��Ʈ�ʿ� Ÿ���� 2���� �׸���
	DrawEllipseInHDC(hDCMem, wParam, lParam);

	SelectObject(hDCMem, hOldPen);

	// ȭ�� ��¿� DC�� Pen �ڵ� ����
	DeleteObject(hPen);
	ReleaseDC(hWnd, hDC);
}

// Ÿ���� Ư�� HDC�� �׸�
void DrawEllipseInHDC(HDC tHDC, WPARAM wParam, LPARAM lParam)
{
	// �߽��� ã��
	int centerX = (LOWORD(wParam) + LOWORD(lParam)) / 2;
	int centerY = (HIWORD(wParam) + HIWORD(lParam)) / 2;

	// X,Y�� ���� �� ���� ���ϱ�
	int ellipseAxisX = centerX - LOWORD(wParam);
	int ellipseAxisY = centerY - HIWORD(wParam);

	// ����
	double angle;
	// ���� ��ġ
	int newX, newY;

	// ���� ��ġ
	int oldX = LOWORD(lParam);
	int oldY = centerY;

	// 1���� ȸ���ϸ鼭 Ÿ�� �׸���
	for (int i = 0; i <= 360; i++)
	{
		// Ÿ�� ���� ����
		angle = 2 * 3.1416 * i / 360;
		newX = centerX + ellipseAxisX * cos(angle);
		newY = centerY + ellipseAxisY * sin(angle);

		// ù ��°�� ���� ������ �����Ƿ� ����
		if (i != 0)
		{
			DrawLineInHDC(tHDC, MAKEWPARAM(oldX, oldY), MAKELPARAM(newX, newY));
		}

		// ���� ��ġ�� �ٽ� ���� ��ġ�� �ű�.
		oldX = newX;
		oldY = newY;
	}
}

// �� �׸��� ���� ����
void DrawLineProcess(HWND hWnd, HDC& hDCMem, WPARAM wParam, LPARAM lParam, DRAW_DETAIL_INFORMATION drawDetailInformation)
{
	HDC hDC = GetDC(hWnd);
	HPEN hPen = CreatePen(PS_SOLID, drawDetailInformation.width, drawDetailInformation.color);
	// ������ ȭ�鿡 ���� 1���� �׸���
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
	DrawLineInHDC(hDC, wParam, lParam);
	SelectObject(hDC, hOldPen);
	// ��� ��Ʈ�ʿ� ���� 2���� �׸���
	hOldPen = (HPEN)SelectObject(hDCMem, hPen);
	DrawLineInHDC(hDCMem, wParam, lParam);
	SelectObject(hDCMem, hOldPen);
	// ȭ�� ��¿� DC�� Pen �ڵ� ����
	DeleteObject(hPen);
	ReleaseDC(hWnd, hDC);
}

// ���� Ư�� HDC�� �׸�
void DrawLineInHDC(HDC tHDC, WPARAM wParam, LPARAM lParam)
{
	MoveToEx(tHDC, LOWORD(wParam), HIWORD(wParam), NULL);
	LineTo(tHDC, LOWORD(lParam), HIWORD(lParam));
}