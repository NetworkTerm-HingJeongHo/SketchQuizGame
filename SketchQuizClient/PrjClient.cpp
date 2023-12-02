#include "stdafx.h"

// ---- ���� ���� (�α����� ����) ----- //
_TCHAR input_result[256]; // input ��� ������ �迭
_TCHAR ID_NICKNAME[256]; // stdafx.h ���Ͽ� ���� �ּҿ� �����ϱ� ����

// Ȩ â ����
int channel;	//udp ä�� ��������. stdafx.h ���Ͽ� ���� �ּҿ� �����ϱ� ����

//-------------------------------//

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// �̺�Ʈ ����(���� ��ȣ, ����?����)
	g_hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if (g_hReadEvent == NULL) return 1;
	g_hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (g_hWriteEvent == NULL) return 1;

	// ���� ���� �ʱ�ȭ(�Ϻ�)
	g_chatmsg.type = TYPE_CHAT;
	g_drawlinemsg.type = TYPE_DRAWLINE;
	g_drawlinemsg.color = RGB(255, 0, 0);
	g_erasepicmsg.type = TYPE_ERASEPIC;

	// ===== ��ȣ =====
	g_drawellipsemsg.type = TYPE_DRAWELLIPSE;
	g_drawellipsemsg.color = RGB(255, 0, 0);
	//

	g_hInstance = hInstance;


	//-------����--------//
	// Ȩ ����ȭ�� ������ Ŭ���� ���?
	WNDCLASS wcMain = { 0 };
	wcMain.lpfnWndProc = HomeWndProc;
	wcMain.hInstance = hInstance;
	wcMain.lpszClassName = _T("MainWindowClass");
	RegisterClass(&wcMain);

	// �α��� ������ Ŭ���� ���?
	WNDCLASS wcLogin = { 0 };
	wcLogin.lpfnWndProc = LoginWndProc;
	wcLogin.hInstance = hInstance;
	wcLogin.lpszClassName = _T("LoginWindowClass");
	RegisterClass(&wcLogin);

	// Ȩ_��������_�Է� ������ Ŭ���� ���?
	WNDCLASS wcHome_Notice = { 0 };
	wcHome_Notice.lpfnWndProc = Home_NoticeWndProc;
	wcHome_Notice.hInstance = hInstance;
	wcHome_Notice.lpszClassName = _T("Home_NoticeWindowClass");
	RegisterClass(&wcHome_Notice);

	// Ȩ_��������_��й��?�Է� ������ Ŭ���� ���?
	WNDCLASS wcHome_Pass = { 0 };
	wcHome_Pass.lpfnWndProc = Home_PassWndProc;
	wcHome_Pass.hInstance = hInstance;
	wcHome_Pass.lpszClassName = _T("Home_PassWindowClass");
	RegisterClass(&wcHome_Pass);

	//------------------//
	
	// ���� ������(ù ȭ��) ����
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, MainWndProc, 0, 0, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("MainWindow"), NULL };
	RegisterClassEx(&wcex);
	g_hMainWindow = CreateWindow(_T("MainWindow"), _T("Main Window"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 200, NULL, NULL, hInstance, NULL);

	ShowWindow(g_hMainWindow, nCmdShow);
	UpdateWindow(g_hMainWindow);

	// �޽��� ����
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!IsDialogMessage(g_hMainWindow, &msg)) // ��ȭ ���� �޽��� ó��
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// �̺�Ʈ ��ü ����
	CloseHandle(g_hReadEvent);
	CloseHandle(g_hWriteEvent);
	// ���� ����
	WSACleanup();
	return 0;
}

// ���� ������ ���ν���
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		// '�׸���' ��ư ����
		CreateWindow(_T("BUTTON"), _T("�׸���"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 10, 10, 100, 30, hWnd, (HMENU)1, g_hInstance, NULL);

		// '�α���' ��ư ����
		CreateWindow(_T("BUTTON"), _T("�α���"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 120, 10, 100, 30, hWnd, (HMENU)2, g_hInstance, NULL);

		// '��ŷ' ��ư ���� (�� ��°�� ��ġ)
		CreateWindow(_T("BUTTON"), _T("��ŷ"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 230, 10, 100, 30, hWnd, (HMENU)4, g_hInstance, NULL);

		// '����' ��ư ���� (�� ��°�� ��ġ)
		CreateWindow(_T("BUTTON"), _T("����"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 340, 10, 100, 30, hWnd, (HMENU)3, g_hInstance, NULL);
		break;
	}
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == 1) // '�׸���' ��ư Ŭ��
		{
			CreateAndShowDialog(hWnd);

			ShowWindow(hWnd, SW_HIDE);
		}
		//---���� ----//
		else if (LOWORD(wParam) == 2) // '�α���' ��ư Ŭ��
		{
			CreateAndShowWindow_Login(hwndLogin);
		}
		else if (LOWORD(wParam) == 3) // '����' ��ư Ŭ��
		{
			CreateAndShowWindow_Home(hwndHome); // ���� ����
		}

		//-----------//
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

// ��ȭ���� ���ν���(�׸��� â)
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hChkIsIPv6;
	static HWND hEditIPaddr;
	static HWND hEditPort;
	static HWND hChkIsUDP;
	static HWND hBtnConnect;
	static HWND hBtnSendFile; // ���� �������� ����
	static HWND hBtnSendMsg; // ���� �������� ����
	static HWND hEditMsg;
	static HWND hEditStatus; // ���� �������� ����
	static HWND hBtnErasePic; // ���� �������� ����
	static HWND hStaticDummy;

	// ========= ���� =========
	static HWND hTimer;    // Ÿ�̸� ǥ�� 
	static HWND hWord;     // ���þ� ǥ��
	static HWND hBtnStart; // ���� ���� ��ư


	// ========= ���� =========
	static HWND hBtnPenColor;
	static HWND hLineWidth;
	static HWND hDlgChannel;

	// ========= ��ȣ =========
	static HWND hFigureSelect;	// �׸� ���� ����
	//

	switch (uMsg) {
	case WM_INITDIALOG:
		// ��Ʈ�� �ڵ� ���?
		hChkIsIPv6 = GetDlgItem(hDlg, IDC_ISIPV6);
		hEditIPaddr = GetDlgItem(hDlg, IDC_IPADDR);
		hEditPort = GetDlgItem(hDlg, IDC_PORT);
		hChkIsUDP = GetDlgItem(hDlg, IDC_ISUDP);
		hBtnConnect = GetDlgItem(hDlg, IDC_CONNECT);
		hBtnSendFile = GetDlgItem(hDlg, IDC_SENDFILE);
		g_hBtnSendFile = hBtnSendFile; // ���� ������ ����
		hBtnSendMsg = GetDlgItem(hDlg, IDC_SENDMSG);
		g_hBtnSendMsg = hBtnSendMsg; // ���� ������ ����
		hEditMsg = GetDlgItem(hDlg, IDC_MSG);
		hEditStatus = GetDlgItem(hDlg, IDC_STATUS);
		g_hEditStatus = hEditStatus; // ���� ������ ����
		hBtnErasePic = GetDlgItem(hDlg, IDC_ERASEPIC);
		g_hBtnErasePic = hBtnErasePic; // ���� ������ ����
		hStaticDummy = GetDlgItem(hDlg, IDC_DUMMY);



		// ========= ���� =========
		g_hTimerStatus = GetDlgItem(hDlg, IDC_EDIT_TIMER);  // Ÿ�̸� ǥ���ϴ� EditText �κ� 
		g_hWordStatus = GetDlgItem(hDlg, IDC_EDIT_WORD);    // ���þ� ǥ���ϴ� EditText �κ�
		hBtnStart = GetDlgItem(hDlg, IDC_GAMESTART);        // ���� ��ŸƮ ��ư
		g_hDrawDlg = hDlg; // ���̾�α�?���������� ����(���߿� ä�� ǥ���ϴ� �뵵�� ���?
		WideCharToMultiByte(CP_ACP, 0, ID_NICKNAME, 256, NICKNAME_CHAR, 256, NULL, NULL); //_TCHAR �� ���ڿ��� char* �� ���ڿ��� ����

		// ========= ���� =========
		hBtnPenColor = GetDlgItem(hDlg, IDC_PENCOLOR);
		g_hBtnPenColor = hBtnPenColor; // ���� ������ ����

		hLineWidth = GetDlgItem(hDlg, IDC_LINEWIDTH);
		g_hLineWidth = hLineWidth; // ���� ������ ����

		// ========= ��ȣ =========
		// �׸� ���� �����ϴ� �ڵ鷯�� ���?���� ������ ����
		hFigureSelect = GetDlgItem(hDlg, IDC_FIGURE);
		g_hFigureSelect = hFigureSelect;
		//

		// ��Ʈ�� �ʱ�ȭ
		SetDlgItemText(hDlg, IDC_IPADDR, SERVERIP4);
		SetDlgItemInt(hDlg, IDC_PORT, SERVERPORT, FALSE);
		EnableWindow(g_hBtnSendFile, FALSE);
		EnableWindow(g_hBtnSendMsg, FALSE);
		SendMessage(hEditMsg, EM_SETLIMITTEXT, SIZE_DAT / 2, 0);
		EnableWindow(g_hBtnErasePic, FALSE);

		// ========= ���� =========
		EnableWindow(g_hBtnPenColor, FALSE);
		EnableWindow(g_hLineWidth, FALSE);

		AddLineWidthOption(hDlg);

		// ========= ��ȣ =========
		AddFigureOption(hDlg);
		EnableWindow(g_hFigureSelect, FALSE);
		//

		// ========= ���� =========
		EnableWindow(hBtnStart, FALSE); //���� ���� ������ ��Ȱ��ȭ
		//

		
		// ������ Ŭ���� ���?
		WNDCLASS wndclass;
		wndclass.style = CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc = ChildWndProc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = g_hInstance;
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName = _T("MyWndClass");
		wndclass.lpszClassName = _T("MyWndClass");
		if (!RegisterClass(&wndclass)) exit(1);

		// �ڽ� ������ ����
		RECT rect; GetWindowRect(hStaticDummy, &rect);
		POINT pt; pt.x = rect.left; pt.y = rect.top;
		ScreenToClient(hDlg, &pt);
		g_hDrawWnd = CreateWindow(_T("MyWndClass"), _T(""), WS_CHILD,
			pt.x, pt.y, rect.right - rect.left, rect.bottom - rect.top,
			hDlg, (HMENU)NULL, g_hInstance, NULL);
		if (g_hDrawWnd == NULL) exit(1);
		ShowWindow(g_hDrawWnd, SW_SHOW);
		UpdateWindow(g_hDrawWnd);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_ISIPV6:
			g_isIPv6 = SendMessage(hChkIsIPv6, BM_GETCHECK, 0, 0);
			if (g_isIPv6 == false)
				SetDlgItemText(hDlg, IDC_IPADDR, SERVERIP4);
			else
				SetDlgItemText(hDlg, IDC_IPADDR, SERVERIP6);
			return TRUE;
		case IDC_CONNECT:
			// ��Ʈ�� ���� ���?
			GetDlgItemTextA(hDlg, IDC_IPADDR, g_ipaddr, sizeof(g_ipaddr));
			g_port = GetDlgItemInt(hDlg, IDC_PORT, NULL, TRUE);
			g_isIPv6 = SendMessage(hChkIsIPv6, BM_GETCHECK, 0, 0);
			g_isUDP = SendMessage(hChkIsUDP, BM_GETCHECK, 0, 0);
			//=============  ���� ===============//
			// ä�ο� ���� UDP, TCP üũ ���� �ٲٱ�
			switch (channel) {
				case CHANNEL_TCP: //TCP�� UDP ä�� falsem
					g_isUDP = false;
					break;
				case CHANNEL_UDP1: //UDP�� udp ��ư true
					g_isUDP = true;
					break;
				case CHANNEL_UDP2: //UDP�� udp ��ư true
					g_isUDP = true;
				default:
					break;
			}
			//=====================================//

			// ���� ���?������ ����
			g_hClientThread = CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);
			if (g_hClientThread == NULL) exit(0);
			// ���� ���� ���� ��ٸ�?
			while (g_bCommStarted == false);
			// ��Ʈ�� ���� ����
			EnableWindow(hChkIsIPv6, FALSE);
			EnableWindow(hEditIPaddr, FALSE);
			EnableWindow(hEditPort, FALSE);
			EnableWindow(hChkIsUDP, FALSE);
			EnableWindow(hBtnConnect, FALSE);
			EnableWindow(g_hBtnSendFile, TRUE);
			EnableWindow(g_hBtnSendMsg, TRUE);
			SetFocus(hEditMsg);
			EnableWindow(g_hBtnErasePic, TRUE);

			// ========= ���� =========
			EnableWindow(g_hBtnPenColor, TRUE);
			EnableWindow(g_hLineWidth, TRUE);

			// ========= ���� =========
			EnableWindow(hBtnStart, FALSE); // ���� ���� ��ư Ȱ��ȭ
			

			// ������ ���� ä�� �޽��� �б� �ϷḦ ��ٸ�?
			WaitForSingleObject(g_hReadEvent, INFINITE);
			isMessageQueue = TRUE;
			// ���ο� ä�� �޽����� ���?���� �ϷḦ �˸�
			g_chatmsg.type = TYPE_NOTY;
			
			snprintf(g_chatmsg.msg, sizeof(g_chatmsg), "[%s] ���� �����Ͽ����ϴ�.", NICKNAME_CHAR);
			SetEvent(g_hWriteEvent);

			// ========= ��ȣ =========
			EnableWindow(g_hFigureSelect, TRUE);
			//

			return TRUE;
		case IDC_SENDFILE:
			MessageBox(NULL, _T("���� �������� �ʾҽ��ϴ�."), _T("�˸�"), MB_ICONERROR);
			// 1. GetOpenFileName() ������ API�� �̿��ؼ� ���� ���� ��ȭ���ڸ� ����
			// ������ ������ �����Ѵ�. 2. ������ ������ �о ������ �����Ѵ�.
			return TRUE;
		case IDC_SENDMSG:
			g_chatmsg.type = TYPE_CHAT;
			// ������ ���� ä�� �޽��� �б� �ϷḦ ��ٸ�?
			WaitForSingleObject(g_hReadEvent, INFINITE);
			// ���ο� ä�� �޽����� ���?���� �ϷḦ �˸�
			GetDlgItemTextA(hDlg, IDC_MSG, g_chatmsg.msg, SIZE_DAT);
			SetEvent(g_hWriteEvent);
			// �Էµ� �ؽ�Ʈ ��ü�� ���� ǥ��
			SendMessage(hEditMsg, EM_SETSEL, 0, -1);
			return TRUE;
		case IDC_ERASEPIC:
			send(g_sock, (char*)&g_erasepicmsg, SIZE_TOT, 0);
			return TRUE;
		case IDCANCEL:

			// ������ ���� ä�� �޽��� �б� �ϷḦ ��ٸ�?
			WaitForSingleObject(g_hReadEvent, INFINITE);
			// ���ο� ä�� �޽����� ���?���� �ϷḦ �˸�
			snprintf(g_chatmsg.msg, sizeof(g_chatmsg), "[%s]���� �����Ͽ����ϴ�.", NICKNAME_CHAR);
			SetEvent(g_hWriteEvent);

			closesocket(g_sock);
			EndDialog(hDlg, IDCANCEL);
			//ShowWindow(hDlg, SW_HIDE); 
			//ShowWindow(hwndHome, SW_SHOW);

			//CreateRankDlg(hDlg);
			return TRUE;
		//	======== ���� ==========
		case IDC_PENCOLOR:
			SelectPenColor(&g_clientDrawDetailInformation);
			return TRUE;
		case IDC_LINEWIDTH:
			SelectLineWidth(hDlg, &g_clientDrawDetailInformation);
			return TRUE;

		// ========= ��ȣ ===========
		case IDC_FIGURE:
			SelectFigureOption(hDlg, g_currentSelectFigureMode);
			// "���찳" ��忡����?���� ���� �Ұ������� ����
			if (g_currentSelectFigureMode == MODE_ERASE)
			{
				EnableWindow(g_hBtnPenColor, FALSE);
				// ���� ���?����
				if (!g_isBeforeModeErase)
				{
					g_isBeforeModeErase = true;
					g_lastSelectColor = g_clientDrawDetailInformation.color;
					g_clientDrawDetailInformation.color = RGB(255, 255, 255);
				}
			}
			else
			{
				if (g_isBeforeModeErase)
				{
					g_isBeforeModeErase = false;
					g_clientDrawDetailInformation.color = g_lastSelectColor;
					EnableWindow(g_hBtnPenColor, TRUE);
				}
			}
			return TRUE;
		// ========== ���� ============
		case IDC_GAMESTART:
			gameStart(g_hTimerStatus, g_hWordStatus);
		}
		//
	}
	return FALSE;
}

// �ڽ� ������ ���ν���(�׸��� ����)
LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	HPEN hPen, hOldPen;
	PAINTSTRUCT ps;
	static int cx, cy;
	static HBITMAP hBitmap;
	static HDC hDCMem;
	static int x0, y0;
	static int x1, y1;
	static bool bDrawing;

	switch (uMsg) {
	case WM_SIZE:
		// ȭ�� ��¿�?DC �ڵ� ���?
		hDC = GetDC(hWnd);
		// ���?��Ʈ�ʰ� �޸� DC ����
		cx = LOWORD(lParam);
		cy = HIWORD(lParam);
		hBitmap = CreateCompatibleBitmap(hDC, cx, cy);
		hDCMem = CreateCompatibleDC(hDC);
		SelectObject(hDCMem, hBitmap);
		// ���?��Ʈ�� �������?ä��
		SelectObject(hDCMem, GetStockObject(WHITE_BRUSH));
		SelectObject(hDCMem, GetStockObject(WHITE_PEN));
		Rectangle(hDCMem, 0, 0, cx, cy);
		// ȭ�� ��¿�?DC �ڵ� ����
		ReleaseDC(hWnd, hDC);
		return 0;
	case WM_PAINT:
		// ȭ�� ��¿�?DC �ڵ� ���?
		hDC = BeginPaint(hWnd, &ps);
		// ���?��Ʈ���� ȭ�鿡 ����
		BitBlt(hDC, 0, 0, cx, cy, hDCMem, 0, 0, SRCCOPY);
		// ȭ�� ��¿�?DC �ڵ� ����
		EndPaint(hWnd, &ps);
		return 0;

	case WM_LBUTTONDOWN:
		// ���콺 Ŭ�� ��ǥ ���?
		x0 = LOWORD(lParam);
		y0 = HIWORD(lParam);
		bDrawing = true;
		return 0;
		// ======= ��ȣ =======
	case WM_MOUSEMOVE:
		if (bDrawing && g_bCommStarted) {
			if (g_currentSelectFigureMode == MODE_ERASE || g_currentSelectFigureMode == MODE_LINE)
			{
				// ���콺 Ŭ�� ��ǥ ���?
				x1 = LOWORD(lParam);
				y1 = HIWORD(lParam);
				// �� �׸��� �޽��� ������
				g_drawlinemsg.x0 = x0;
				g_drawlinemsg.y0 = y0;
				g_drawlinemsg.x1 = x1;
				g_drawlinemsg.y1 = y1;

				// ���� �޽����� ���� ���� �����?���� ����
				g_drawlinemsg.color = g_clientDrawDetailInformation.color;
				g_drawlinemsg.width = g_clientDrawDetailInformation.width;

				//sendMsgLen(g_sock, sizeof(g_drawlinemsg));
				//sendn(g_sock, (char*)&g_drawlinemsg, sizeof(g_drawlinemsg), 0);
				sendn(g_sock, (char*)&g_drawlinemsg, SIZE_TOT, 0, serveraddr, g_isUDP);
				// ���콺 Ŭ�� ��ǥ ����
				x0 = x1;
				y0 = y1;
			}
		}
		return 0;
	case WM_LBUTTONUP:
		// ������ ������ �Ϸ�Ǿ���?��
		if (g_bCommStarted)
		{
			switch (g_currentSelectFigureMode)
			{
			// "���찳" ���?
			case MODE_ERASE:
				break;
			// "Ÿ��" �׸��� ���?
			case MODE_ELLIPSE:
				g_drawellipsemsg.x0 = x0;
				g_drawellipsemsg.y0 = y0;
				g_drawellipsemsg.x1 = LOWORD(lParam);
				g_drawellipsemsg.y1 = HIWORD(lParam);
				g_drawellipsemsg.color = g_clientDrawDetailInformation.color;
				g_drawellipsemsg.width = g_clientDrawDetailInformation.width;
				//sendMsgLen(g_sock, sizeof(g_drawellipsemsg));
				//sendn(g_sock, (char*)&g_drawellipsemsg, sizeof(g_drawellipsemsg), 0);
				sendn(g_sock, (char*)&g_drawellipsemsg, SIZE_TOT, 0, serveraddr, g_isUDP);
				break;

			// "�簢��" �׸��� ���?
			case MODE_RECTANGLE:
				break;

			// "�ﰢ��" �׸��� ���?
			case MODE_TRIANGLE:
				break;
			default:
				break;
			}
		}
		bDrawing = false;
		return 0;
	// ======== ��ȣ ==========
	// �� �׸��� �޽��� ����
	case WM_DRAWLINE:
		DrawLineProcess(hWnd, hDCMem, wParam, lParam, g_serverDrawDetailInformation);
		return 0;

	// Ÿ�� �׸��� �޽��� ����
	case WM_DRAWELLIPSE:
		DrawEllipseProcess(hWnd, hDCMem, wParam, lParam, g_serverDrawDetailInformation);
		return 0;

	// Ư�� �κ� ���� �����?������ �޽��� ����
	case WM_ERASEALITTLE:
		DrawLineProcess(hWnd, hDCMem, wParam, lParam, g_serverDrawDetailInformation);
		return 0;
		//
	case WM_ERASEPIC:
		// ���?��Ʈ�� �������?ä��
		SelectObject(hDCMem, GetStockObject(WHITE_BRUSH));
		SelectObject(hDCMem, GetStockObject(WHITE_PEN));
		Rectangle(hDCMem, 0, 0, cx, cy);
		// WM_PAINT �޽��� ���� ����
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case WM_DESTROY:
		DeleteDC(hDCMem);
		DeleteObject(hBitmap);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//// ---- ���� ���� (�α����� ����) ----- //
//_TCHAR input_result[256]; // input ���?������ �迭
//_TCHAR ID_NICKNAME[256]; // stdafx.h ���Ͽ� ���� �ּҿ� �����ϱ� ����
//
//// Ȩ â ����
//int channel;	//udp ä�� ��������. stdafx.h ���Ͽ� ���� �ּҿ� �����ϱ� ����
//
////-------------------------------//

// ---- ���� ------------------- //


//-------------------------------//

// �α��� ������ ���ν��� (�α��� ����) -----------------------------------------------------------------------------------//
LRESULT CALLBACK LoginWndProc(HWND hwndLogin, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CREATE:
		// �α��� ȭ�� �ʱ�ȭ �� ��Ʈ�� ����
																								   //x,y,width,height
		CreateWindow(_T("STATIC"), _T("����ġ����"), WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 500, 100, 300, 100, hwndLogin, NULL, NULL, NULL); // ����ġ���� Ÿ��Ʋ
		CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 400, 300, 400, 40, hwndLogin, (HMENU)ID_ID_INPUT, NULL, NULL);
		CreateWindow(_T("BUTTON"), _T("�ߺ�Ȯ��"), WS_VISIBLE | WS_CHILD, 850, 300, 100, 40, hwndLogin, (HMENU)ID_DUPLICATION_BUTTON, NULL, NULL); // �ߺ�Ȯ�� ��ư
		//CreateWindow(_T("STATIC"), _T("���?������ ID�Դϴ�!"), WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 400, 360, 400, 40, hwndLogin, NULL, NULL, NULL); // ���?������ ID�Դϴ�!
		CreateWindow(_T("BUTTON"), _T("�α���"), WS_VISIBLE | WS_CHILD | WS_DISABLED, 400, 500, 500, 100, hwndLogin, (HMENU)ID_LOGIN_BUTTON, NULL, NULL); // �α��� ��ư (ó�� ��Ȱ��ȭ)
		CreateWindow(_T("BUTTON"), _T("���ư���"), WS_VISIBLE | WS_CHILD, 100, 100, 100, 30, hwndLogin, (HMENU)ID_BACKHOME_BUTTON, NULL, NULL);
		break;

	case WM_COMMAND:
		// ��ư Ŭ�� �̺�Ʈ ó��
		switch (LOWORD(wParam)) {
			// '���ư���' ��ư Ŭ�� ó��
		case ID_BACKHOME_BUTTON:
			// �α��� â�� �����?����â�� ���̰� �ؾ��ϴµ� �ϴ� �α���â ����
			ShowWindow(hwndLogin, SW_HIDE);
			break;

		case ID_LOGIN_BUTTON: // �α��� ��ư�� Ŭ������ ��

			_tcscpy(ID_NICKNAME, input_result); // ���� �Է��� ID ����
			MessageBox(hwndLogin, ID_NICKNAME, _T("���� ȭ������ �̵��մϴ�."), MB_OK);

			CreateAndShowWindow_Home(hwndHome); // ���� ���� �� ���̰��ϱ�
			ShowWindow(hwndLogin, SW_HIDE);
			break;

		case ID_ID_INPUT: // ID�� �Է����ּ���! �Է����� ��
			break;

		case ID_DUPLICATION_BUTTON: //�ߺ� Ȯ�� ��ư Ŭ������ ��, �ߺ� Ȯ���ϱ�
			_TCHAR userId[256]; // �̹� �ִ� �������̵�?
			_tcscpy(userId, _T("abc123")); // Copy the string "abc123" into userId
			GetDlgItemText(hwndLogin, ID_ID_INPUT, input_result, sizeof(input_result));

			// ���� �ִ� Id��, �Է��� ���̵� ���� ��
			if (_tcscmp(userId, input_result) == 0 && _tcscmp(userId, _T("")))
			{
				MessageBox(hwndLogin, _T("�̹� �ִ� ���̵��Դϴ�. �ٸ� ���̵� ������ּ���?"), _T("�ߺ� Ȯ�� ���?), MB_OK);

				// �̹� �ִ� ���̵��� ���?�α��� ��ư ��Ȱ��ȭ
				EnableWindow(GetDlgItem(hwndLogin, ID_LOGIN_BUTTON), FALSE);
			}
			else
			{
				MessageBox(hwndLogin, _T("���?������ ���̵��Դϴ�."), _T("�ߺ� Ȯ�� ���?), MB_OK);

				// ���?������ ���̵��� ���?�α��� ��ư Ȱ��ȭ
				EnableWindow(GetDlgItem(hwndLogin, ID_LOGIN_BUTTON), TRUE);
			}
			break;

		default:
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwndLogin, msg, wParam, lParam);
	}
	return 0;
}

//----------------------------------------------------------------------------------------------------------------------//


//-------------------------------------Ȩ ������ ���ν��� -----------------------------------------------------------------------//
// ������ ���ν��� (����)
LRESULT CALLBACK HomeWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CREATE:
		// Ȩ ���� ȭ�� �ʱ�ȭ �� ��Ʈ�� ����
																							   //x,y,width,height
		CreateWindow(_T("STATIC"), ID_NICKNAME, WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 900, 10, 100, 30, hwnd, NULL, NULL, NULL); // ���� id ���?
		CreateWindow(_T("STATIC"), _T("�� �ݰ����ϴ�!"), WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 1000, 10, 200, 30, hwnd, NULL, NULL, NULL); // id �� �ݰ����ϴ�!
		CreateWindow(_T("STATIC"), _T("���ڻ��� ����"), WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 50, 50, 1150, 100, hwnd, NULL, NULL, NULL); // ����ġ���� Ÿ��Ʋ

		CreateWindow(_T("BUTTON"), _T("���� ����"), WS_VISIBLE | WS_CHILD, 1042, 185, 174, 54, hwnd, (HMENU)ID_NOTICE_BUTTON, NULL, NULL); // ���� ����

		CreateWindow(_T("BUTTON"), _T("TCP ä�� ����"), WS_VISIBLE | WS_CHILD, 300, 200, 640, 100, hwnd, (HMENU)ID_CHANNEL_A_BUTTON, NULL, NULL); // ä�� A ����
		CreateWindow(_T("BUTTON"), _T("UDP ä��1 ����"), WS_VISIBLE | WS_CHILD, 300, 350, 640, 100, hwnd, (HMENU)ID_CHANNEL_B_BUTTON, NULL, NULL); // ä�� B ����

		//CreateWindow(L"BUTTON", L"�游���?, WS_VISIBLE | WS_CHILD, 282, 600, 320, 67, hwnd, (HMENU)ID_BACKHOME_BUTTON, NULL, NULL); // �� �����?
		CreateWindow(_T("BUTTON"), _T("UDP ä��2 ����"), WS_VISIBLE | WS_CHILD, 300, 500, 640, 100, hwnd, (HMENU)ID_CHANNEL_RANDOM_BUTTON, NULL, NULL); // ���� ����


		CreateWindow(_T("BUTTON"), _T("���ư���"), WS_VISIBLE | WS_CHILD, 100, 100, 100, 30, hwnd, (HMENU)ID_BACKHOME_BUTTON, NULL, NULL); // ���ư���
		break;

	case WM_COMMAND:
		// ��ư Ŭ�� �̺�Ʈ ó��
		switch (LOWORD(wParam)) {
			// '���ư���' ��ư Ŭ�� ó��
		case ID_BACKHOME_BUTTON:
			// �α��� â�� �����?���� â�� �ٽ� ���̰� ��
		/*	ShowWindow(hwnd, SW_SHOW);*/
			ShowWindow(hwnd, SW_HIDE);
			break;

		case ID_NOTICE_BUTTON: //[���� ����] ��ư Ŭ����
			CreateAndShowWindow_Home_Pass(hwndHome_Pass); // ������ ��й��?�Է� â ����
			break;

		case ID_CHANNEL_A_BUTTON: // TCP ä�� ��ư Ŭ����
			channel = CHANNEL_TCP;	// tcp ä�� ���� 0���� ����
			ShowWindow(g_hDrawDlg, SW_SHOW);
			CreateAndShowDialog(hwnd);
			break;
		case ID_CHANNEL_B_BUTTON: // UDP ä��1 ��ư Ŭ����
			channel = CHANNEL_UDP1;	//udp ä�� ���� 1�� ����
			CreateAndShowDialog(hwnd);
			break;
		case ID_CHANNEL_RANDOM_BUTTON: // UDP ä��2 ��ư Ŭ����
			channel = CHANNEL_UDP2;	//udp ä�� ���� 2�� ����
			CreateAndShowDialog(hwnd);
			break;
		default:
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

//----------------------------------------------------------------------------------------------------------------------//


//------------------------------------- Ȩ �������� ������ ���ν��� -----------------------------------------------------------------------//
// 
 _TCHAR input_notice_result[256]; // input ���?������ �迭
// 
// Ȩ �������� ������ ���ν���
LRESULT CALLBACK Home_NoticeWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CREATE:
		// �α��� ȭ�� �ʱ�ȭ �� ��Ʈ�� ����
																								   //x,y,width,height
		CreateWindow(_T("STATIC"), _T("�������� �Է�"), WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 200, 50, 200, 50, hwnd, NULL, NULL, NULL); // ����ġ���� Ÿ��Ʋ
		CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 100, 150, 400, 50, hwnd, (HMENU)ID_NOTICE_INPUT, NULL, NULL);	// �������� �Է� input
		CreateWindow(_T("BUTTON"), _T("Ȯ��"), WS_VISIBLE | WS_CHILD, 180, 300, 120, 50, hwnd, (HMENU)ID_OK_BUTTON, NULL, NULL); // Ȯ�� ��ư
		CreateWindow(_T("BUTTON"), _T("���?), WS_VISIBLE | WS_CHILD, 320, 300, 120, 50, hwnd, (HMENU)ID_CANCLE_BUTTON, NULL, NULL);		// ���?��ư
		break;

	case WM_COMMAND:
		// ��ư Ŭ�� �̺�Ʈ ó��
		switch (LOWORD(wParam)) {
			// '���? ��ư Ŭ�� ó��
		case ID_CANCLE_BUTTON:
			// Ȩ �������� �Է�â�� �����?���� â�� �ٽ� ���̰� ��
		/*	ShowWindow(hwnd, SW_SHOW);*/
			ShowWindow(hwnd, SW_HIDE);
			break;
		case ID_OK_BUTTON: //Ȯ�� ��ư Ŭ���� - input ������ �����?
			

			GetDlgItemText(hwnd, ID_NOTICE_INPUT, input_notice_result, sizeof(input_notice_result));
			MessageBox(hwnd, input_notice_result, _T("�������� ���� ����"), MB_OK); // �������� ������ ���� ����
			BoradcaseSendToNotice(input_notice_result); //���۰�����

			//ShowWindow(hwnd, SW_HIDE);	// �������� �Է�â �ݱ�
			break;
		default:
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

//----------------------------------------------------------------------------------------------------------------------//


//------------------------------------- Ȩ �������� ��й��?������ ���ν��� -----------------------------------------------------------------------//
// Ȩ �������� ������ ���ν���
LRESULT CALLBACK Home_PassWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CREATE:
		// �α��� ȭ�� �ʱ�ȭ �� ��Ʈ�� ����
																								   //x,y,width,height
		CreateWindow(_T("STATIC"), _T("������ ��й�ȣ��?�Է����ּ���."), WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 100, 50, 400, 50, hwnd, NULL, NULL, NULL); // ������ ��й��?Ÿ��Ʋ
		CreateWindow(_T("EDIT"), _T("��й��?�Է��ϱ�"), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 100, 150, 400, 50, hwnd, (HMENU)ID_PASSWORD_INPUT, NULL, NULL);	// ��й��?�Է� input
		CreateWindow(_T("BUTTON"), _T("Ȯ��"), WS_VISIBLE | WS_CHILD, 180, 300, 120, 50, hwnd, (HMENU)ID_OK_BUTTON, NULL, NULL); // Ȯ�� ��ư
		CreateWindow(_T("BUTTON"), _T("���?), WS_VISIBLE | WS_CHILD, 320, 300, 120, 50, hwnd, (HMENU)ID_CANCLE_BUTTON, NULL, NULL);		// ���?��ư
		break;

	case WM_COMMAND:
		// ��ư Ŭ�� �̺�Ʈ ó��
		switch (LOWORD(wParam)) {
			// '���? ��ư Ŭ�� ó��
		case ID_CANCLE_BUTTON:
			// Ȩ �������� ��й��?�Է�â�� �����?���� â�� �ٽ� ���̰� ��
		/*	ShowWindow(hwnd, SW_SHOW);*/
			ShowWindow(hwnd, SW_HIDE);
			break;
		case ID_OK_BUTTON: //Ȯ�� ��ư Ŭ���� - input ������ �����?
			_TCHAR password[256]; // �̹� �ִ� �������̵�?
			_tcscpy(password, _T("abc123")); // Copy the string "abc123" into userId

			GetDlgItemText(hwnd, ID_PASSWORD_INPUT, input_result, sizeof(input_result));


			// ��й�ȣ��?��ġ�ϴ��� ��
			if (_tcscmp(password, input_result) == 0 )
			{
				CreateAndShowWindow_Home_Notice(hwndHome_Notice); // �������� �Է� ȭ�� �����ֱ�
				ShowWindow(hwnd, SW_HIDE);
			}
			else
			{
				MessageBox(hwndHome_Pass, _T("��й�ȣ��?��ġ���� �ʽ��ϴ�. �ٽ� �Է����ּ���."), _T("����"), MB_OK);
				
			}
			break;
		default:
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

//----------------------------------------------------------------------------------------------------------------------//

//--------------------------------------------------지??---------------------------------------------------------------//
// ?�켓 ?�신 ?�레???�수 (0) - 로그?�할???�켓 ?�신?�기
// ?�라?�언?��? ?�이???�신
DWORD WINAPI LoginProcessClient()
{

	int retval;
	// socket()
	g_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (g_sock == INVALID_SOCKET) err_quit("socket()");
	
	// connect() : 기존 TCP ?�결???�닌 ?�로 만든�?
	SOCKADDR_IN Loginserveraddr;
	ZeroMemory(&Loginserveraddr, sizeof(Loginserveraddr));

	Loginserveraddr.sin_family = AF_INET; //server???�???�정
	Loginserveraddr.sin_addr.s_addr = inet_addr(SERVERIP4_CHAR); //server???�???�정
	Loginserveraddr.sin_port = htons(SERVERPORT); //server???�???�정
	retval = connect(g_sock, (SOCKADDR*)&Loginserveraddr, sizeof(Loginserveraddr)); //?�결?�때, ?�버?�켓 ?�보�?준?? -> establishied ?�태
	if (retval == SOCKET_ERROR) err_quit("connect()");

	
	
	// ?�이???�신???�용??변??
	//char buf[BUFSIZE + 1]="TCP?�결?�라??;
	//int len;

	//retval = send(g_sock, buf, strlen(buf), 0); //버퍼?�이즈로 보내�?
	//if (retval == SOCKET_ERROR) {
	//	err_display("send()");
	//	//break;
	//}
	

	int len;
	len = sizeof(NICKNAME_CHAR);

	// 고정 ?�기 ?�이???�송
	retval = sendn(g_sock, (char*)&NICKNAME_CHAR, BUFSIZE, 0, serveraddr, false);

	printf("[TCP ?�라?�언?? %d바이?��? 보냈?�니??\n", retval);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		//break;
	}

	//retval = sendn(g_sock, (char*)&len, sizeof(int), 0);
	//// 가변 ?�기 ?�이???�송
	//retval = sendn(g_sock, (char*)&g_chatmsg, len, 0);
	if (retval == SOCKET_ERROR)
		return 0;

	return 0;
	/*
	//// ?�버?� ?�이???�신
	while (1) {
	//	// ?�이??보내�?
		retval = send(g_sock, buf, strlen(buf), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			//break;
		}
		printf("[TCP ?�라?�언?? %d바이?��? 보냈?�니??\n", retval);

		// ?�이??받기
		retval = recvn(g_sock, (char*)&buf, BUFSIZE, 0, serveraddr, false); //
		//retval = recvn(g_sock, buf, retval, 0); // retval�??�시 ?��? ?�유 : ?��? 보낸만큼 ?�시 받기 ?�해?�이?? (10byte보냈?�면 10만큼 받게 N???�정?��?�?
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			//break;
		}
		else if (retval == 0)
			//break;

		// 받�? ?�이??출력
		buf[retval] = '\0';
		printf("[TCP ?�라?�언?? %d바이?��? 받았?�니??\n", retval);
		printf("[받�? ?�이?? %s\n", buf);
	}

	//HANDLE hThread[2];

	//hThread[1] = CreateThread(NULL, 0, LoginProcessClient, (LPVOID)1, 0, NULL);
	//WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	// //?�속 종료
	//WSACleanup();
	*/

}


//----------------------------------------------------------------------------------------------------------------------//


// ���� ���?������ �Լ�(1) - ����
DWORD WINAPI ClientMain(LPVOID arg)
{
	int retval;

	if (g_isIPv6 == false && g_isUDP == false) { // TCP/IPv4 ����
		// socket()
		g_sock = socket(AF_INET, SOCK_STREAM, 0);
		if (g_sock == INVALID_SOCKET) err_quit("socket()");

		// connect()
		memset(&serveraddr, 0, sizeof(serveraddr));
		serveraddr.sin_family = AF_INET;
		inet_pton(AF_INET, g_ipaddr, &serveraddr.sin_addr);
		serveraddr.sin_port = htons(g_port);
		retval = connect(g_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
		if (retval == SOCKET_ERROR) err_quit("connect()");
	}
	else if (g_isIPv6 == true && g_isUDP == false) { // TCP/IPv6 ����
		// socket()
		g_sock = socket(AF_INET6, SOCK_STREAM, 0);
		if (g_sock == INVALID_SOCKET) err_quit("socket()");

		// connect()
		struct sockaddr_in6 serveraddr;
		memset(&serveraddr, 0, sizeof(serveraddr));
		serveraddr.sin6_family = AF_INET6;
		inet_pton(AF_INET6, g_ipaddr, &serveraddr.sin6_addr);
		serveraddr.sin6_port = htons(g_port);
		retval = connect(g_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
		if (retval == SOCKET_ERROR) err_quit("connect()");
	}
	else if (g_isIPv6 == false && g_isUDP == true) { // UDP/IPv4 ����
		//========================================== ���� ==========================================//
		
		//--------------------- UDP ���� 1 ----------------------//
		if (channel == CHANNEL_UDP1) { //UDP ä�� 1 �̶��?
			MessageBox(NULL, _T("�����̰� �������� UDP ä��1 IPv4 Ŭ���̾�Ʈ ������"), _T("�˸�"), MB_ICONERROR);
			// socket()
			g_sock = socket(AF_INET, SOCK_DGRAM, 0);
			if (g_sock == INVALID_SOCKET) err_quit("socket()");

			// ��Ƽĳ��Ʈ �׷� ���� - (UDP�� ���ἳ���� ���� �����Ƿ�, connet() ���ʿ�)
			struct ip_mreq mreq;
			mreq.imr_multiaddr.s_addr = inet_addr(SERVERIP4_CHAR_UDP1); // �����ϰų� Ż���� IPv4 ��Ƽ�ɽ�Ʈ address(�ּ�) (������ ���Ƹ�)
			mreq.imr_interface.s_addr = htonl(INADDR_ANY);		// ���� ip address (��)
			retval = setsockopt(g_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
				(char*)&mreq, sizeof(mreq));
			if (retval == SOCKET_ERROR) err_quit("setsockopt()");

			// ���� �ּ� ����ü �ʱ�ȭ
			ZeroMemory(&serveraddr, sizeof(serveraddr));
			serveraddr.sin_family = AF_INET;
			serveraddr.sin_addr.s_addr = inet_addr(SERVERIP4_CHAR_UDP1);
			serveraddr.sin_port = htons(SERVERPORT);

			// ������ ��ſ�?�����?����
			char buf[BUFSIZE + 1] = "hello, I'am UDP JIAN. UDP Channel1 !!";
			int len;

			// ������ ������
			retval = sendto(g_sock, buf, strlen(buf), 0,
				(SOCKADDR*)&serveraddr, sizeof(serveraddr));
			if (retval == SOCKET_ERROR) {
				err_display("sendto()");
			}

			// ��Ƽĳ��Ʈ �׷� Ż�� (�����?udp ä�� x�Ҷ� �̷��� �ϼ�)
			retval = setsockopt(g_sock, IPPROTO_IP, IP_DROP_MEMBERSHIP,
				(char*)&mreq, sizeof(mreq));
			if (retval == SOCKET_ERROR) err_quit("setsockopt()");
		}
		//------------------------------------------------------//

		//--------------------- UDP ���� 2 ----------------------//
		else if (channel == CHANNEL_UDP2) { //UDP ä�� 2���?
			MessageBox(NULL, _T("�����̰� �������� UDP ä��2 IPv4 Ŭ���̾�Ʈ ������"), _T("�˸�"), MB_ICONERROR);
			// socket()
			g_sock = socket(AF_INET, SOCK_DGRAM, 0);
			if (g_sock == INVALID_SOCKET) err_quit("socket()");

			// ��Ƽĳ��Ʈ �׷� ���� - (UDP�� ���ἳ���� ���� �����Ƿ�, connet() ���ʿ�)
			struct ip_mreq mreq;
			mreq.imr_multiaddr.s_addr = inet_addr(SERVERIP4_CHAR_UDP2); // �����ϰų� Ż���� IPv4 ��Ƽ�ɽ�Ʈ address(�ּ�) (������ ���Ƹ�)
			mreq.imr_interface.s_addr = htonl(INADDR_ANY);		// ���� ip address (��)
			retval = setsockopt(g_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
				(char*)&mreq, sizeof(mreq));
			if (retval == SOCKET_ERROR) err_quit("setsockopt()");

			// ���� �ּ� ����ü �ʱ�ȭ
			ZeroMemory(&serveraddr, sizeof(serveraddr));
			serveraddr.sin_family = AF_INET;
			serveraddr.sin_addr.s_addr = inet_addr(SERVERIP4_CHAR_UDP2);
			serveraddr.sin_port = htons(SERVERPORT);

			// ������ ��ſ�?�����?����
			char buf[BUFSIZE + 1] = "hello, I'am UDP JIAN. UDP Channel2 !!";
			int len;

			// ������ ������
			retval = sendto(g_sock, buf, strlen(buf), 0,
				(SOCKADDR*)&serveraddr, sizeof(serveraddr));
			if (retval == SOCKET_ERROR) {
				err_display("sendto()");
			}

			// ��Ƽĳ��Ʈ �׷� Ż�� (�����?udp ä�� x�Ҷ� �̷��� �ϼ�)
			retval = setsockopt(g_sock, IPPROTO_IP, IP_DROP_MEMBERSHIP,
				(char*)&mreq, sizeof(mreq));
			if (retval == SOCKET_ERROR) err_quit("setsockopt()");

		}
		//------------------------------------------------------//

		//==========================================================================================//
	}
	else if (g_isIPv6 == true && g_isUDP == true) { // UDP/IPv6 ����
		MessageBox(NULL, _T("���� �������� �ʾҽ��ϴ�."), _T("�˸�"), MB_ICONERROR);
		exit(1);
	}
	MessageBox(NULL, _T("������ �����߽��ϴ�."), _T("�˸�"), MB_ICONINFORMATION);

	// �б� & ���� ������ ����
	HANDLE hThread[2];
	hThread[0] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, WriteThread, NULL, 0, NULL);
	if (hThread[0] == NULL || hThread[1] == NULL) exit(1);
	g_bCommStarted = true;

	// ������ ���� ���?(�� �� �ϳ��� ������ ������)
	retval = WaitForMultipleObjects(2, hThread, FALSE, INFINITE);
	retval -= WAIT_OBJECT_0;
	if (retval == 0)
		TerminateThread(hThread[1], 1);
	else
		TerminateThread(hThread[0], 1);
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);

	MessageBox(NULL, _T("������ ������ϴ�?"), _T("�˸�"), MB_ICONERROR);
	EnableWindow(g_hBtnSendFile, FALSE);
	EnableWindow(g_hBtnSendMsg, FALSE);
	EnableWindow(g_hBtnErasePic, FALSE);
	g_bCommStarted = false;
	closesocket(g_sock);
	return 0;
}

// ���� ���?������ �Լ�(2) - ������ ����
DWORD WINAPI ReadThread(LPVOID arg)
{
	int retval;
	COMM_MSG comm_msg;
	CHAT_MSG* chat_msg;
	DRAWLINE_MSG* drawline_msg;
	ERASEPIC_MSG* erasepic_msg;
	char reciever[20], sender[20], tmp[5];


	// ------ ���� --------
	char senderName[256];
	char recieverName[256];
	char sendMsg[256];
	char word[10];

	// ====== ��ȣ ========
	DRAWELLIPSE_MSG* drawEllipse_msg;
	int serveraddrLen;
	int len;
	//

	while (1) {

		//// ������ �ޱ�(���� ����)
		//retval = recvn(g_sock, (char*)&len, sizeof(int), 0);
		//if (retval == SOCKET_ERROR) 
		//{
		//	err_display("recvn()");
		//	break;
		//}
		//else if (retval == 0)
		//{
		//	break;
		//}

		//// ������ �ޱ�(���� ����)
		//retval = recvn(g_sock, (char*)&comm_msg, len, 0);
		//if (retval == 0 || retval == SOCKET_ERROR) {
		//	err_display("recv()");
		//	break;
		//}
		//else if (retval == 0)
		//{
		//	break;
		//}
		// 

		retval = recvn(g_sock, (char*)&comm_msg, BUFSIZE, 0, serveraddr, g_isUDP);
		if (retval == 0 || retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
		{
			break;
		}

		switch (comm_msg.type)
		{
		// ============ ���� ==========
		case TYPE_CHAT:
			if (comm_msg.type == TYPE_CHAT) {
				chat_msg = (CHAT_MSG*)&comm_msg;
				sscanf(chat_msg->msg, "{%[^}]%*s%s", senderName, sendMsg);

				if (strncmp(sendMsg, "/w ", 3) == 0) {
					sscanf(sendMsg, "%s %s %s", tmp, sender, reciever);
					if (strcmp(reciever, NICKNAME_CHAR) == 0) {
						MySendFile(sender, reciever, chat_msg->msg);
						DisplayText("%s\r\n", chat_msg->msg);
					}
				}
				else {
					DisplayText("%s\r\n", chat_msg->msg);
				}

				WideCharToMultiByte(CP_ACP, 0, quizWord[roundNum], 10, word, 10, NULL, NULL);
				if (strcmp(sendMsg, word) == 0) {  // ���þ ���� ���? �������� ����ϰ�?�� ���� ����

					DisplayText("[%s] �����Դϴ�!\r\n", word);
					newRound();
				}
			}
			break;
		case TYPE_NOTY:
			chat_msg = (CHAT_MSG*)&comm_msg;
			DisplayText("%s\r\n", chat_msg->msg);
			break;
		case TYPE_DRAWLINE:
			drawline_msg = (DRAWLINE_MSG*)&comm_msg;
			// ============ ���� ============
			g_serverDrawDetailInformation.width = drawline_msg->width;
			// ==============================
			g_serverDrawDetailInformation.color = drawline_msg->color;
			SendMessage(g_hDrawWnd, WM_DRAWLINE,
				MAKEWPARAM(drawline_msg->x0, drawline_msg->y0),
				MAKELPARAM(drawline_msg->x1, drawline_msg->y1));
			break;
		// ======== ��ȣ ==========
		case TYPE_DRAWELLIPSE:
			drawEllipse_msg = (DRAWELLIPSE_MSG*)&comm_msg;
			g_serverDrawDetailInformation.width = drawEllipse_msg->width;
			g_serverDrawDetailInformation.color = drawEllipse_msg->color;
			SendMessage(g_hDrawWnd, WM_DRAWELLIPSE,
				MAKEWPARAM(drawEllipse_msg->x0, drawEllipse_msg->y0),
				MAKELPARAM(drawEllipse_msg->x1, drawEllipse_msg->y1));
			break;
		case TYPE_ERASEPIC:
			erasepic_msg = (ERASEPIC_MSG*)&comm_msg;
			SendMessage(g_hDrawWnd, WM_ERASEPIC, 0, 0);
			break;

		default:
			break;
		}


	}
	return 0;
}

// ���� ���?������ �Լ�(3) - ������ �۽�
DWORD WINAPI WriteThread(LPVOID arg)
{
	int retval, len;
	char* nickName;

	// ������ ������ ���?
	while (1) {
		// ���� �Ϸ� ��ٸ���?
		WaitForSingleObject(g_hWriteEvent, INFINITE);
		// ���ڿ� ���̰� 0�̸� ������ ����
		if (strlen(g_chatmsg.msg) == 0) {
			// [�޽��� ����] ��ư Ȱ��ȭ
			EnableWindow(g_hBtnSendMsg, TRUE);
			// �б� �Ϸ� �˸���
			SetEvent(g_hReadEvent);
			continue;
		}
		// ============ ��ȣ ===========
		// ������ ������

		char sendMsg[256];
		if (g_chatmsg.type == TYPE_CHAT) {
			snprintf(sendMsg, sizeof(sendMsg), "{%s} %s", NICKNAME_CHAR, g_chatmsg.msg);
			strcpy(g_chatmsg.msg, sendMsg);

		}
		len = sizeof(g_chatmsg);

		// ���� ũ�� ������ ����
		retval = sendn(g_sock, (char*)&g_chatmsg, BUFSIZE, 0, serveraddr, g_isUDP);

		//retval = sendn(g_sock, (char*)&len, sizeof(int), 0);
		//// ���� ũ�� ������ ����
		//retval = sendn(g_sock, (char*)&g_chatmsg, len, 0);
		if (retval == SOCKET_ERROR) break;


		// [�޽��� ����] ��ư Ȱ��ȭ
		EnableWindow(g_hBtnSendMsg, TRUE);
		// �б� �Ϸ� �˸���
		SetEvent(g_hReadEvent);
	}
	return 0;
}

// ����Ʈ ��Ʈ�� ���?�Լ�
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
