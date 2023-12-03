#include "stdafx.h"

// ---- 占쏙옙占쏙옙 占쏙옙占쏙옙 (占싸깍옙占쏙옙占쏙옙 占쏙옙占쏙옙) ----- //
_TCHAR input_result[256]; // input 占쏙옙占 占쏙옙占쏙옙占쏙옙 占썼열
_TCHAR ID_NICKNAME[256]; // stdafx.h 占쏙옙占싹울옙 占쏙옙占쏙옙 占쌍소울옙 占쏙옙占쏙옙占싹깍옙 占쏙옙占쏙옙

// 홈 창 占쏙옙占쏙옙
int channel;	//udp 채占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙. stdafx.h 占쏙옙占싹울옙 占쏙옙占쏙옙 占쌍소울옙 占쏙옙占쏙옙占싹깍옙 占쏙옙占쏙옙

//-------------------------------//

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 占쏙옙占쏙옙 占십깍옙화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 占싱븝옙트 占쏙옙占쏙옙(占쏙옙占쏙옙 占쏙옙호, 占쏙옙占싫 占쏙옙占쏙옙)
	g_hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if (g_hReadEvent == NULL) return 1;
	g_hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (g_hWriteEvent == NULL) return 1;

	// 占쏙옙占쏙옙 占쏙옙占쏙옙 占십깍옙화(占싹븝옙)
	g_chatmsg.type = TYPE_CHAT;
	g_drawlinemsg.type = TYPE_DRAWLINE;
	g_drawlinemsg.color = RGB(255, 0, 0);
	g_erasepicmsg.type = TYPE_ERASEPIC;

	// ===== 占쏙옙호 =====
	g_drawellipsemsg.type = TYPE_DRAWELLIPSE;
	g_drawellipsemsg.color = RGB(255, 0, 0);
	//

	g_hInstance = hInstance;


	//-------占쏙옙占쏙옙--------//
	// 홈 占쏙옙占쏙옙화占쏙옙 占쏙옙占쏙옙占쏙옙 클占쏙옙占쏙옙 占쏙옙占
	WNDCLASS wcMain = { 0 };
	wcMain.lpfnWndProc = HomeWndProc;
	wcMain.hInstance = hInstance;
	wcMain.lpszClassName = _T("MainWindowClass");
	RegisterClass(&wcMain);

	// 占싸깍옙占쏙옙 占쏙옙占쏙옙占쏙옙 클占쏙옙占쏙옙 占쏙옙占
	WNDCLASS wcLogin = { 0 };
	wcLogin.lpfnWndProc = LoginWndProc;
	wcLogin.hInstance = hInstance;
	wcLogin.lpszClassName = _T("LoginWindowClass");
	RegisterClass(&wcLogin);

	// 홈_占쏙옙占쏙옙占쏙옙占쏙옙_占쌉뤄옙 占쏙옙占쏙옙占쏙옙 클占쏙옙占쏙옙 占쏙옙占
	WNDCLASS wcHome_Notice = { 0 };
	wcHome_Notice.lpfnWndProc = Home_NoticeWndProc;
	wcHome_Notice.hInstance = hInstance;
	wcHome_Notice.lpszClassName = _T("Home_NoticeWindowClass");
	RegisterClass(&wcHome_Notice);

	// 홈_占쏙옙占쏙옙占쏙옙占쏙옙_占쏙옙橘占싫 占쌉뤄옙 占쏙옙占쏙옙占쏙옙 클占쏙옙占쏙옙 占쏙옙占
	WNDCLASS wcHome_Pass = { 0 };
	wcHome_Pass.lpfnWndProc = Home_PassWndProc;
	wcHome_Pass.hInstance = hInstance;
	wcHome_Pass.lpszClassName = _T("Home_PassWindowClass");
	RegisterClass(&wcHome_Pass);

	//------------------//
	
	// 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙(첫 화占쏙옙) 占쏙옙占쏙옙
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, MainWndProc, 0, 0, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("MainWindow"), NULL };
	RegisterClassEx(&wcex);
	g_hMainWindow = CreateWindow(_T("MainWindow"), _T("Main Window"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 200, NULL, NULL, hInstance, NULL);

	ShowWindow(g_hMainWindow, nCmdShow);
	UpdateWindow(g_hMainWindow);

	// 占쌨쏙옙占쏙옙 占쏙옙占쏙옙
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!IsDialogMessage(g_hMainWindow, &msg)) // 占쏙옙화 占쏙옙占쏙옙 占쌨쏙옙占쏙옙 처占쏙옙
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// 占싱븝옙트 占쏙옙체 占쏙옙占쏙옙
	CloseHandle(g_hReadEvent);
	CloseHandle(g_hWriteEvent);
	// 占쏙옙占쏙옙 占쏙옙占쏙옙
	WSACleanup();
	return 0;
}

// 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占싸쏙옙占쏙옙
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		// '占쌓몌옙占쏙옙' 占쏙옙튼 占쏙옙占쏙옙
		CreateWindow(_T("BUTTON"), _T("占쌓몌옙占쏙옙"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 10, 10, 100, 30, hWnd, (HMENU)1, g_hInstance, NULL);

		// '占싸깍옙占쏙옙' 占쏙옙튼 占쏙옙占쏙옙
		CreateWindow(_T("BUTTON"), _T("占싸깍옙占쏙옙"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 120, 10, 100, 30, hWnd, (HMENU)2, g_hInstance, NULL);

		// '占쏙옙킹' 占쏙옙튼 占쏙옙占쏙옙 (占쏙옙 占쏙옙째占쏙옙 占쏙옙치)
		CreateWindow(_T("BUTTON"), _T("占쏙옙킹"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 230, 10, 100, 30, hWnd, (HMENU)4, g_hInstance, NULL);

		// '占쏙옙占쏙옙' 占쏙옙튼 占쏙옙占쏙옙 (占쏙옙 占쏙옙째占쏙옙 占쏙옙치)
		CreateWindow(_T("BUTTON"), _T("占쏙옙占쏙옙"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 340, 10, 100, 30, hWnd, (HMENU)3, g_hInstance, NULL);
		break;
	}
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == 1) // '占쌓몌옙占쏙옙' 占쏙옙튼 클占쏙옙
		{
			CreateAndShowDialog(hWnd);

			ShowWindow(hWnd, SW_HIDE);
		}
		//---占쏙옙占쏙옙 ----//
		else if (LOWORD(wParam) == 2) // '占싸깍옙占쏙옙' 占쏙옙튼 클占쏙옙
		{
			CreateAndShowWindow_Login(hwndLogin);
		}
		else if (LOWORD(wParam) == 3) // '占쏙옙占쏙옙' 占쏙옙튼 클占쏙옙
		{
			CreateAndShowWindow_Home(hwndHome); // 占쏙옙占쏙옙 占쏙옙占쏙옙
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

// 占쏙옙화占쏙옙占쏙옙 占쏙옙占싸쏙옙占쏙옙(占쌓몌옙占쏙옙 창)
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hChkIsIPv6;
	static HWND hEditIPaddr;
	static HWND hEditPort;
	static HWND hChkIsUDP;
	static HWND hBtnConnect;
	static HWND hBtnSendFile; // 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙
	static HWND hBtnSendMsg; // 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙
	static HWND hEditMsg;
	static HWND hEditStatus; // 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙
	static HWND hBtnErasePic; // 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙
	static HWND hStaticDummy;

	// ========= 占쏙옙占쏙옙 =========
	static HWND hTimer;    // 타占싱몌옙 표占쏙옙 
	static HWND hWord;     // 占쏙옙占시억옙 표占쏙옙
	static HWND hBtnStart; // 占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙튼


	// ========= 占쏙옙占쏙옙 =========
	static HWND hBtnPenColor;
	static HWND hLineWidth;
	static HWND hDlgChannel;

	// ========= 占쏙옙호 =========
	static HWND hFigureSelect;	// 占쌓몌옙 占쏙옙占쏙옙 占쏙옙占쏙옙
	//

	switch (uMsg) {
	case WM_INITDIALOG:
		// 占쏙옙트占쏙옙 占쌘듸옙 占쏙옙占
		hChkIsIPv6 = GetDlgItem(hDlg, IDC_ISIPV6);
		hEditIPaddr = GetDlgItem(hDlg, IDC_IPADDR);
		hEditPort = GetDlgItem(hDlg, IDC_PORT);
		hChkIsUDP = GetDlgItem(hDlg, IDC_ISUDP);
		hBtnConnect = GetDlgItem(hDlg, IDC_CONNECT);
		hBtnSendFile = GetDlgItem(hDlg, IDC_SENDFILE);
		g_hBtnSendFile = hBtnSendFile; // 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙
		hBtnSendMsg = GetDlgItem(hDlg, IDC_SENDMSG);
		g_hBtnSendMsg = hBtnSendMsg; // 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙
		hEditMsg = GetDlgItem(hDlg, IDC_MSG);
		hEditStatus = GetDlgItem(hDlg, IDC_STATUS);
		g_hEditStatus = hEditStatus; // 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙
		hBtnErasePic = GetDlgItem(hDlg, IDC_ERASEPIC);
		g_hBtnErasePic = hBtnErasePic; // 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙
		hStaticDummy = GetDlgItem(hDlg, IDC_DUMMY);



		// ========= 占쏙옙占쏙옙 =========
		g_hTimerStatus = GetDlgItem(hDlg, IDC_EDIT_TIMER);  // 타占싱몌옙 표占쏙옙占싹댐옙 EditText 占싸븝옙 
		g_hWordStatus = GetDlgItem(hDlg, IDC_EDIT_WORD);    // 占쏙옙占시억옙 표占쏙옙占싹댐옙 EditText 占싸븝옙
		hBtnStart = GetDlgItem(hDlg, IDC_GAMESTART);        // 占쏙옙占쏙옙 占쏙옙타트 占쏙옙튼
		g_hDrawDlg = hDlg; // 占쏙옙占싱억옙慣占 占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙(占쏙옙占쌩울옙 채占쏙옙 표占쏙옙占싹댐옙 占쎈도占쏙옙 占쏙옙占)
		WideCharToMultiByte(CP_ACP, 0, ID_NICKNAME, 256, NICKNAME_CHAR, 256, NULL, NULL); //_TCHAR 占쏙옙 占쏙옙占쌘울옙占쏙옙 char* 占쏙옙 占쏙옙占쌘울옙占쏙옙 占쏙옙占쏙옙

		// ========= 占쏙옙占쏙옙 =========
		hBtnPenColor = GetDlgItem(hDlg, IDC_PENCOLOR);
		g_hBtnPenColor = hBtnPenColor; // 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙

		hLineWidth = GetDlgItem(hDlg, IDC_LINEWIDTH);
		g_hLineWidth = hLineWidth; // 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙

		// ========= 占쏙옙호 =========
		// 占쌓몌옙 占쏙옙占쏙옙 占쏙옙占쏙옙占싹댐옙 占쌘들러占쏙옙 占쏙옙底 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙
		hFigureSelect = GetDlgItem(hDlg, IDC_FIGURE);
		g_hFigureSelect = hFigureSelect;
		//

		// 占쏙옙트占쏙옙 占십깍옙화
		SetDlgItemText(hDlg, IDC_IPADDR, SERVERIP4);
		SetDlgItemInt(hDlg, IDC_PORT, SERVERPORT, FALSE);
		EnableWindow(g_hBtnSendFile, FALSE);
		EnableWindow(g_hBtnSendMsg, FALSE);
		SendMessage(hEditMsg, EM_SETLIMITTEXT, SIZE_DAT / 2, 0);
		EnableWindow(g_hBtnErasePic, FALSE);

		// ========= 占쏙옙占쏙옙 =========
		EnableWindow(g_hBtnPenColor, FALSE);
		EnableWindow(g_hLineWidth, FALSE);

		AddLineWidthOption(hDlg);

		// ========= 占쏙옙호 =========
		AddFigureOption(hDlg);
		EnableWindow(g_hFigureSelect, FALSE);
		//

		// ========= 占쏙옙占쏙옙 =========
		EnableWindow(hBtnStart, FALSE); //占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙활占쏙옙화
		//

		
		// 占쏙옙占쏙옙占쏙옙 클占쏙옙占쏙옙 占쏙옙占
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

		// 占쌘쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙
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
			// 占쏙옙트占쏙옙 占쏙옙占쏙옙 占쏙옙占
			GetDlgItemTextA(hDlg, IDC_IPADDR, g_ipaddr, sizeof(g_ipaddr));
			g_port = GetDlgItemInt(hDlg, IDC_PORT, NULL, TRUE);
			g_isIPv6 = SendMessage(hChkIsIPv6, BM_GETCHECK, 0, 0);
			g_isUDP = SendMessage(hChkIsUDP, BM_GETCHECK, 0, 0);
			//=============  占쏙옙占쏙옙 ===============//
			// 채占싸울옙 占쏙옙占쏙옙 UDP, TCP 체크 占쏙옙占쏙옙 占쌕꾸깍옙
			switch (channel) {
				case CHANNEL_TCP: //TCP占쏙옙 UDP 채占쏙옙 falsem
					g_isUDP = false;
					break;
				case CHANNEL_UDP1: //UDP占쏙옙 udp 占쏙옙튼 true
					g_isUDP = true;
					break;
				case CHANNEL_UDP2: //UDP占쏙옙 udp 占쏙옙튼 true
					g_isUDP = true;
				default:
					break;
			}
			//=====================================//

			// 占쏙옙占쏙옙 占쏙옙占 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙
			g_hClientThread = CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);
			if (g_hClientThread == NULL) exit(0);
			// 占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙摸占
			while (g_bCommStarted == false);
			// 占쏙옙트占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙
			EnableWindow(hChkIsIPv6, FALSE);
			EnableWindow(hEditIPaddr, FALSE);
			EnableWindow(hEditPort, FALSE);
			EnableWindow(hChkIsUDP, FALSE);
			EnableWindow(hBtnConnect, FALSE);
			EnableWindow(g_hBtnSendFile, TRUE);
			EnableWindow(g_hBtnSendMsg, TRUE);
			SetFocus(hEditMsg);
			EnableWindow(g_hBtnErasePic, TRUE);

			// ========= 占쏙옙占쏙옙 =========
			EnableWindow(g_hBtnPenColor, TRUE);
			EnableWindow(g_hLineWidth, TRUE);

			// ========= 占쏙옙占쏙옙 =========
			EnableWindow(hBtnStart, FALSE); // 占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙튼 활占쏙옙화
			

			// 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙 채占쏙옙 占쌨쏙옙占쏙옙 占싻깍옙 占싹료를 占쏙옙摸占
			WaitForSingleObject(g_hReadEvent, INFINITE);
			isMessageQueue = TRUE;
			// 占쏙옙占싸울옙 채占쏙옙 占쌨쏙옙占쏙옙占쏙옙 占쏙옙占 占쏙옙占쏙옙 占싹료를 占싯몌옙
			g_chatmsg.type = TYPE_NOTY;
			
			snprintf(g_chatmsg.msg, sizeof(g_chatmsg), "[%s] 占쏙옙占쏙옙 占쏙옙占쏙옙占싹울옙占쏙옙占싹댐옙.", NICKNAME_CHAR);
			SetEvent(g_hWriteEvent);
			// ===================================

			// ========= 占쏙옙호 =========
			EnableWindow(g_hFigureSelect, TRUE);
			//

			return TRUE;
		case IDC_SENDFILE:
			MessageBox(NULL, _T("占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙 占십았쏙옙占싹댐옙."), _T("占싯몌옙"), MB_ICONERROR);
			// 1. GetOpenFileName() 占쏙옙占쏙옙占쏙옙 API占쏙옙 占싱울옙占쌔쇽옙 占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙화占쏙옙占쌘몌옙 占쏙옙占쏙옙
			// 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占싼댐옙. 2. 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占싻어서 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占싼댐옙.
			return TRUE;
		case IDC_SENDMSG:
			g_chatmsg.type = TYPE_CHAT;
			// 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙 채占쏙옙 占쌨쏙옙占쏙옙 占싻깍옙 占싹료를 占쏙옙摸占
			WaitForSingleObject(g_hReadEvent, INFINITE);
			// 占쏙옙占싸울옙 채占쏙옙 占쌨쏙옙占쏙옙占쏙옙 占쏙옙占 占쏙옙占쏙옙 占싹료를 占싯몌옙
			GetDlgItemTextA(hDlg, IDC_MSG, g_chatmsg.msg, SIZE_DAT);
			SetEvent(g_hWriteEvent);
			// 占쌉력듸옙 占쌔쏙옙트 占쏙옙체占쏙옙 占쏙옙占쏙옙 표占쏙옙
			SendMessage(hEditMsg, EM_SETSEL, 0, -1);
			return TRUE;
		case IDC_ERASEPIC:
			send(g_sock, (char*)&g_erasepicmsg, SIZE_TOT, 0);
			return TRUE;
 		case IDCANCEL:

			// 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙 채占쏙옙 占쌨쏙옙占쏙옙 占싻깍옙 占싹료를 占쏙옙摸占
			WaitForSingleObject(g_hReadEvent, INFINITE);
			// 占쏙옙占싸울옙 채占쏙옙 占쌨쏙옙占쏙옙占쏙옙 占쏙옙占 占쏙옙占쏙옙 占싹료를 占싯몌옙
			snprintf(g_chatmsg.msg, sizeof(g_chatmsg), "[%s]占쏙옙占쏙옙 占쏙옙占쏙옙占싹울옙占쏙옙占싹댐옙.", NICKNAME_CHAR);
			SetEvent(g_hWriteEvent);

			closesocket(g_sock);
			EndDialog(hDlg, IDCANCEL);
			//ShowWindow(hDlg, SW_HIDE); 
			//ShowWindow(hwndHome, SW_SHOW);

			//CreateRankDlg(hDlg);
			return TRUE;
		//	======== 占쏙옙占쏙옙 ==========
		case IDC_PENCOLOR:
			SelectPenColor(&g_clientDrawDetailInformation);
			return TRUE;
		case IDC_LINEWIDTH:
			SelectLineWidth(hDlg, &g_clientDrawDetailInformation);
			return TRUE;

		// ========= 占쏙옙호 ===========
		case IDC_FIGURE:
			SelectFigureOption(hDlg, g_currentSelectFigureMode);
			// "占쏙옙占쎌개" 占쏙옙恙∽옙占쏙옙占 占쏙옙占쏙옙 占쏙옙占쏙옙 占쌀곤옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙
			if (g_currentSelectFigureMode == MODE_ERASE)
			{
				EnableWindow(g_hBtnPenColor, FALSE);
				// 占쏙옙占쏙옙 占쏙옙占 占쏙옙占쏙옙
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
		// ========== 占쏙옙占쏙옙 ============
		case IDC_GAMESTART:
			gameStart(g_hTimerStatus, g_hWordStatus);
		}
		//
	}
	return FALSE;
}

// 占쌘쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占싸쏙옙占쏙옙(占쌓몌옙占쏙옙 占쏙옙占쏙옙)
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
		// 화占쏙옙 占쏙옙쩔占 DC 占쌘듸옙 占쏙옙占
		hDC = GetDC(hWnd);
		// 占쏙옙占 占쏙옙트占십곤옙 占쌨몌옙 DC 占쏙옙占쏙옙
		cx = LOWORD(lParam);
		cy = HIWORD(lParam);
		hBitmap = CreateCompatibleBitmap(hDC, cx, cy);
		hDCMem = CreateCompatibleDC(hDC);
		SelectObject(hDCMem, hBitmap);
		// 占쏙옙占 占쏙옙트占쏙옙 占쏙옙占쏙옙占쏙옙占 채占쏙옙
		SelectObject(hDCMem, GetStockObject(WHITE_BRUSH));
		SelectObject(hDCMem, GetStockObject(WHITE_PEN));
		Rectangle(hDCMem, 0, 0, cx, cy);
		// 화占쏙옙 占쏙옙쩔占 DC 占쌘듸옙 占쏙옙占쏙옙
		ReleaseDC(hWnd, hDC);
		return 0;
	case WM_PAINT:
		// 화占쏙옙 占쏙옙쩔占 DC 占쌘듸옙 占쏙옙占
		hDC = BeginPaint(hWnd, &ps);
		// 占쏙옙占 占쏙옙트占쏙옙占쏙옙 화占썽에 占쏙옙占쏙옙
		BitBlt(hDC, 0, 0, cx, cy, hDCMem, 0, 0, SRCCOPY);
		// 화占쏙옙 占쏙옙쩔占 DC 占쌘듸옙 占쏙옙占쏙옙
		EndPaint(hWnd, &ps);
		return 0;

	case WM_LBUTTONDOWN:
		// 占쏙옙占쎌스 클占쏙옙 占쏙옙표 占쏙옙占
		x0 = LOWORD(lParam);
		y0 = HIWORD(lParam);
		bDrawing = true;
		return 0;
		// ======= 占쏙옙호 =======
	case WM_MOUSEMOVE:
		if (bDrawing && g_bCommStarted) {
			if (g_currentSelectFigureMode == MODE_ERASE || g_currentSelectFigureMode == MODE_LINE)
			{
				// 占쏙옙占쎌스 클占쏙옙 占쏙옙표 占쏙옙占
				x1 = LOWORD(lParam);
				y1 = HIWORD(lParam);
				// 占쏙옙 占쌓몌옙占쏙옙 占쌨쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙
				g_drawlinemsg.x0 = x0;
				g_drawlinemsg.y0 = y0;
				g_drawlinemsg.x1 = x1;
				g_drawlinemsg.y1 = y1;

				// 占쏙옙占쏙옙 占쌨쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙占 占쏙옙占쏙옙 占쏙옙占쏙옙
				g_drawlinemsg.color = g_clientDrawDetailInformation.color;
				g_drawlinemsg.width = g_clientDrawDetailInformation.width;

				//sendMsgLen(g_sock, sizeof(g_drawlinemsg));
				//sendn(g_sock, (char*)&g_drawlinemsg, sizeof(g_drawlinemsg), 0);
				sendn(g_sock, (char*)&g_drawlinemsg, SIZE_TOT, 0, serveraddr, g_isUDP);
				// 占쏙옙占쎌스 클占쏙옙 占쏙옙표 占쏙옙占쏙옙
				x0 = x1;
				y0 = y1;
			}
		}
		return 0;
	case WM_LBUTTONUP:
		// 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占싹뤄옙퓸占쏙옙占 占쏙옙
		if (g_bCommStarted)
		{
			switch (g_currentSelectFigureMode)
			{
			// "占쏙옙占쎌개" 占쏙옙占
			case MODE_ERASE:
				break;
			// "타占쏙옙" 占쌓몌옙占쏙옙 占쏙옙占
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

			// "占썹각占쏙옙" 占쌓몌옙占쏙옙 占쏙옙占
			case MODE_RECTANGLE:
				break;

			// "占쏙각占쏙옙" 占쌓몌옙占쏙옙 占쏙옙占
			case MODE_TRIANGLE:
				break;
			default:
				break;
			}
		}
		bDrawing = false;
		return 0;
	// ======== 占쏙옙호 ==========
	// 占쏙옙 占쌓몌옙占쏙옙 占쌨쏙옙占쏙옙 占쏙옙占쏙옙
	case WM_DRAWLINE:
		DrawLineProcess(hWnd, hDCMem, wParam, lParam, g_serverDrawDetailInformation);
		return 0;

	// 타占쏙옙 占쌓몌옙占쏙옙 占쌨쏙옙占쏙옙 占쏙옙占쏙옙
	case WM_DRAWELLIPSE:
		DrawEllipseProcess(hWnd, hDCMem, wParam, lParam, g_serverDrawDetailInformation);
		return 0;

	// 특占쏙옙 占싸븝옙 占쏙옙占쏙옙 占쏙옙占쏙옙占 占쏙옙占쏙옙占쏙옙 占쌨쏙옙占쏙옙 占쏙옙占쏙옙
	case WM_ERASEALITTLE:
		DrawLineProcess(hWnd, hDCMem, wParam, lParam, g_serverDrawDetailInformation);
		return 0;
	//
	case WM_ERASEPIC:
		// 占쏙옙占 占쏙옙트占쏙옙 占쏙옙占쏙옙占쏙옙占 채占쏙옙
		SelectObject(hDCMem, GetStockObject(WHITE_BRUSH));
		SelectObject(hDCMem, GetStockObject(WHITE_PEN));
		Rectangle(hDCMem, 0, 0, cx, cy);
		// WM_PAINT 占쌨쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙
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

//// ---- 占쏙옙占쏙옙 占쏙옙占쏙옙 (占싸깍옙占쏙옙占쏙옙 占쏙옙占쏙옙) ----- //
//_TCHAR input_result[256]; // input 占쏙옙占 占쏙옙占쏙옙占쏙옙 占썼열
//_TCHAR ID_NICKNAME[256]; // stdafx.h 占쏙옙占싹울옙 占쏙옙占쏙옙 占쌍소울옙 占쏙옙占쏙옙占싹깍옙 占쏙옙占쏙옙
//
//// 홈 창 占쏙옙占쏙옙
//int channel;	//udp 채占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙. stdafx.h 占쏙옙占싹울옙 占쏙옙占쏙옙 占쌍소울옙 占쏙옙占쏙옙占싹깍옙 占쏙옙占쏙옙
//
////-------------------------------//

// ---- 占쏙옙占쏙옙 ------------------- //


//-------------------------------//

// 占싸깍옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占싸쏙옙占쏙옙 (占싸깍옙占쏙옙 占쏙옙占쏙옙) -----------------------------------------------------------------------------------//
LRESULT CALLBACK LoginWndProc(HWND hwndLogin, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CREATE:
		// 占싸깍옙占쏙옙 화占쏙옙 占십깍옙화 占쏙옙 占쏙옙트占쏙옙 占쏙옙占쏙옙
																								   //x,y,width,height
		CreateWindow(_T("STATIC"), _T("占쏙옙占쏙옙치占쏙옙占쏙옙"), WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 500, 100, 300, 100, hwndLogin, NULL, NULL, NULL); // 占쏙옙占쏙옙치占쏙옙占쏙옙 타占쏙옙틀
		CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 400, 300, 400, 40, hwndLogin, (HMENU)ID_ID_INPUT, NULL, NULL);
		CreateWindow(_T("BUTTON"), _T("占쌩븝옙확占쏙옙"), WS_VISIBLE | WS_CHILD, 850, 300, 100, 40, hwndLogin, (HMENU)ID_DUPLICATION_BUTTON, NULL, NULL); // 占쌩븝옙확占쏙옙 占쏙옙튼
		//CreateWindow(_T("STATIC"), _T("占쏙옙占 占쏙옙占쏙옙占쏙옙 ID占쌉니댐옙!"), WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 400, 360, 400, 40, hwndLogin, NULL, NULL, NULL); // 占쏙옙占 占쏙옙占쏙옙占쏙옙 ID占쌉니댐옙!
		CreateWindow(_T("BUTTON"), _T("占싸깍옙占쏙옙"), WS_VISIBLE | WS_CHILD | WS_DISABLED, 400, 500, 500, 100, hwndLogin, (HMENU)ID_LOGIN_BUTTON, NULL, NULL); // 占싸깍옙占쏙옙 占쏙옙튼 (처占쏙옙 占쏙옙활占쏙옙화)
		CreateWindow(_T("BUTTON"), _T("占쏙옙占싣곤옙占쏙옙"), WS_VISIBLE | WS_CHILD, 100, 100, 100, 30, hwndLogin, (HMENU)ID_BACKHOME_BUTTON, NULL, NULL);
		break;

	case WM_COMMAND:
		// 占쏙옙튼 클占쏙옙 占싱븝옙트 처占쏙옙
		switch (LOWORD(wParam)) {
			// '占쏙옙占싣곤옙占쏙옙' 占쏙옙튼 클占쏙옙 처占쏙옙
		case ID_BACKHOME_BUTTON:
			// 占싸깍옙占쏙옙 창占쏙옙 占쏙옙占쏙옙占 占쏙옙占쏙옙창占쏙옙 占쏙옙占싱곤옙 占쌔억옙占싹는듸옙 占싹댐옙 占싸깍옙占쏙옙창 占쏙옙占쏙옙
			ShowWindow(hwndLogin, SW_HIDE);
			break;

		case ID_LOGIN_BUTTON: // 로그인 버튼을 클릭했을 시
			
			_tcscpy(ID_NICKNAME, input_result); // 현재 입력한 ID 저장
			MessageBox(hwndLogin, ID_NICKNAME, _T("메인 화면으로 이동합니다."), MB_OK);

			CreateAndShowWindow_Home(hwndHome); // 占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙 占쏙옙占싱곤옙占싹깍옙
			ShowWindow(hwndLogin, SW_HIDE);
			break;

		case ID_ID_INPUT: // ID占쏙옙 占쌉뤄옙占쏙옙占쌍쇽옙占쏙옙! 占쌉뤄옙占쏙옙占쏙옙 占쏙옙
			break;

		case ID_DUPLICATION_BUTTON: //占쌩븝옙 확占쏙옙 占쏙옙튼 클占쏙옙占쏙옙占쏙옙 占쏙옙, 占쌩븝옙 확占쏙옙占싹깍옙
			_TCHAR userId[256]; // 占싱뱄옙 占쌍댐옙 占쏙옙占쏙옙占쏙옙占싱듸옙?
			_tcscpy(userId, _T("abc123")); // Copy the string "abc123" into userId
			GetDlgItemText(hwndLogin, ID_ID_INPUT, input_result, sizeof(input_result));


			// ---- 로그인 할때 TCP 연결 ---- //
			_tcscpy(ID_NICKNAME, input_result); // 현재 입력한 ID 저장
			WideCharToMultiByte(CP_ACP, 0, ID_NICKNAME, 256, NICKNAME_CHAR, 256, NULL, NULL); //_TCHAR 형 문자열을 char* 형 문자열로 변경
			LoginProcessClient(); //TCP 연결. ->
			// ---------------------------- //
			 
			 
			// 현재 있는 Id와, 입력한 아이디 와의 비교
			if (_tcscmp(userId, input_result) == 0 && _tcscmp(userId, _T("")))
			{
				MessageBox(hwndLogin, _T("占싱뱄옙 占쌍댐옙 占쏙옙占싱듸옙占쌉니댐옙. 占쌕몌옙 占쏙옙占싱듸옙 占쏙옙占쏙옙占쏙옙玲占쏙옙占."), _T("占쌩븝옙 확占쏙옙 占쏙옙占"), MB_OK);

				// 占싱뱄옙 占쌍댐옙 占쏙옙占싱듸옙占쏙옙 占쏙옙占 占싸깍옙占쏙옙 占쏙옙튼 占쏙옙활占쏙옙화
				EnableWindow(GetDlgItem(hwndLogin, ID_LOGIN_BUTTON), FALSE);
			}
			else
			{
				MessageBox(hwndLogin, _T("占쏙옙占 占쏙옙占쏙옙占쏙옙 占쏙옙占싱듸옙占쌉니댐옙."), _T("占쌩븝옙 확占쏙옙 占쏙옙占"), MB_OK);

				// 占쏙옙占 占쏙옙占쏙옙占쏙옙 占쏙옙占싱듸옙占쏙옙 占쏙옙占 占싸깍옙占쏙옙 占쏙옙튼 활占쏙옙화
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


//-------------------------------------홈 占쏙옙占쏙옙占쏙옙 占쏙옙占싸쏙옙占쏙옙 -----------------------------------------------------------------------//
// 占쏙옙占쏙옙占쏙옙 占쏙옙占싸쏙옙占쏙옙 (占쏙옙占쏙옙)
LRESULT CALLBACK HomeWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CREATE:
		// 홈 占쏙옙占쏙옙 화占쏙옙 占십깍옙화 占쏙옙 占쏙옙트占쏙옙 占쏙옙占쏙옙
																							   //x,y,width,height
		CreateWindow(_T("STATIC"), ID_NICKNAME, WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 900, 10, 100, 30, hwnd, NULL, NULL, NULL); // 占쏙옙占쏙옙 id 占쏙옙占
		CreateWindow(_T("STATIC"), _T("占쏙옙 占쌥곤옙占쏙옙占싹댐옙!"), WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 1000, 10, 200, 30, hwnd, NULL, NULL, NULL); // id 占쏙옙 占쌥곤옙占쏙옙占싹댐옙!
		CreateWindow(_T("STATIC"), _T("占쏙옙占쌘삼옙占쏙옙 占쏙옙占쏙옙"), WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 50, 50, 1150, 100, hwnd, NULL, NULL, NULL); // 占쏙옙占쏙옙치占쏙옙占쏙옙 타占쏙옙틀

		CreateWindow(_T("BUTTON"), _T("占쏙옙占쏙옙 占쏙옙占쏙옙"), WS_VISIBLE | WS_CHILD, 1042, 185, 174, 54, hwnd, (HMENU)ID_NOTICE_BUTTON, NULL, NULL); // 占쏙옙占쏙옙 占쏙옙占쏙옙

		CreateWindow(_T("BUTTON"), _T("TCP 채占쏙옙 占쏙옙占쏙옙"), WS_VISIBLE | WS_CHILD, 300, 200, 640, 100, hwnd, (HMENU)ID_CHANNEL_A_BUTTON, NULL, NULL); // 채占쏙옙 A 占쏙옙占쏙옙
		CreateWindow(_T("BUTTON"), _T("UDP 채占쏙옙1 占쏙옙占쏙옙"), WS_VISIBLE | WS_CHILD, 300, 350, 640, 100, hwnd, (HMENU)ID_CHANNEL_B_BUTTON, NULL, NULL); // 채占쏙옙 B 占쏙옙占쏙옙

		//CreateWindow(L"BUTTON", L"占썸만占쏙옙占", WS_VISIBLE | WS_CHILD, 282, 600, 320, 67, hwnd, (HMENU)ID_BACKHOME_BUTTON, NULL, NULL); // 占쏙옙 占쏙옙占쏙옙占
		CreateWindow(_T("BUTTON"), _T("UDP 채占쏙옙2 占쏙옙占쏙옙"), WS_VISIBLE | WS_CHILD, 300, 500, 640, 100, hwnd, (HMENU)ID_CHANNEL_RANDOM_BUTTON, NULL, NULL); // 占쏙옙占쏙옙 占쏙옙占쏙옙


		CreateWindow(_T("BUTTON"), _T("占쏙옙占싣곤옙占쏙옙"), WS_VISIBLE | WS_CHILD, 100, 100, 100, 30, hwnd, (HMENU)ID_BACKHOME_BUTTON, NULL, NULL); // 占쏙옙占싣곤옙占쏙옙
		break;

	case WM_COMMAND:
		// 占쏙옙튼 클占쏙옙 占싱븝옙트 처占쏙옙
		switch (LOWORD(wParam)) {
			// '占쏙옙占싣곤옙占쏙옙' 占쏙옙튼 클占쏙옙 처占쏙옙
		case ID_BACKHOME_BUTTON:
			// 占싸깍옙占쏙옙 창占쏙옙 占쏙옙占쏙옙占 占쏙옙占쏙옙 창占쏙옙 占쌕쏙옙 占쏙옙占싱곤옙 占쏙옙
		/*	ShowWindow(hwnd, SW_SHOW);*/
			ShowWindow(hwnd, SW_HIDE);
			break;

		case ID_NOTICE_BUTTON: //[占쏙옙占쏙옙 占쏙옙占쏙옙] 占쏙옙튼 클占쏙옙占쏙옙
			CreateAndShowWindow_Home_Pass(hwndHome_Pass); // 占쏙옙占쏙옙占쏙옙 占쏙옙橘占싫 占쌉뤄옙 창 占쏙옙占쏙옙
			break;

		case ID_CHANNEL_A_BUTTON: // TCP 채占쏙옙 占쏙옙튼 클占쏙옙占쏙옙
			channel = CHANNEL_TCP;	// tcp 채占쏙옙 占쏙옙占쏙옙 0占쏙옙占쏙옙 占쏙옙占쏙옙
			ShowWindow(g_hDrawDlg, SW_SHOW);
			CreateAndShowDialog(hwnd);
			break;
		case ID_CHANNEL_B_BUTTON: // UDP 채占쏙옙1 占쏙옙튼 클占쏙옙占쏙옙
			channel = CHANNEL_UDP1;	//udp 채占쏙옙 占쏙옙占쏙옙 1占쏙옙 占쏙옙占쏙옙
			CreateAndShowDialog(hwnd);
			break;
		case ID_CHANNEL_RANDOM_BUTTON: // UDP 채占쏙옙2 占쏙옙튼 클占쏙옙占쏙옙
			channel = CHANNEL_UDP2;	//udp 채占쏙옙 占쏙옙占쏙옙 2占쏙옙 占쏙옙占쏙옙
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


//------------------------------------- 홈 占쏙옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占싸쏙옙占쏙옙 -----------------------------------------------------------------------//
// 
 _TCHAR input_notice_result[256]; // input 占쏙옙占 占쏙옙占쏙옙占쏙옙 占썼열
// 
// 홈 占쏙옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占싸쏙옙占쏙옙
LRESULT CALLBACK Home_NoticeWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CREATE:
		// 占싸깍옙占쏙옙 화占쏙옙 占십깍옙화 占쏙옙 占쏙옙트占쏙옙 占쏙옙占쏙옙
																								   //x,y,width,height
		CreateWindow(_T("STATIC"), _T("占쏙옙占쏙옙占쏙옙占쏙옙 占쌉뤄옙"), WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 200, 50, 200, 50, hwnd, NULL, NULL, NULL); // 占쏙옙占쏙옙치占쏙옙占쏙옙 타占쏙옙틀
		CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 100, 150, 400, 50, hwnd, (HMENU)ID_NOTICE_INPUT, NULL, NULL);	// 占쏙옙占쏙옙占쏙옙占쏙옙 占쌉뤄옙 input
		CreateWindow(_T("BUTTON"), _T("확占쏙옙"), WS_VISIBLE | WS_CHILD, 180, 300, 120, 50, hwnd, (HMENU)ID_OK_BUTTON, NULL, NULL); // 확占쏙옙 占쏙옙튼
		CreateWindow(_T("BUTTON"), _T("占쏙옙占"), WS_VISIBLE | WS_CHILD, 320, 300, 120, 50, hwnd, (HMENU)ID_CANCLE_BUTTON, NULL, NULL);		// 占쏙옙占 占쏙옙튼
		break;

	case WM_COMMAND:
		// 占쏙옙튼 클占쏙옙 占싱븝옙트 처占쏙옙
		switch (LOWORD(wParam)) {
			// '占쏙옙占' 占쏙옙튼 클占쏙옙 처占쏙옙
		case ID_CANCLE_BUTTON:
			// 홈 占쏙옙占쏙옙占쏙옙占쏙옙 占쌉뤄옙창占쏙옙 占쏙옙占쏙옙占 占쏙옙占쏙옙 창占쏙옙 占쌕쏙옙 占쏙옙占싱곤옙 占쏙옙
		/*	ShowWindow(hwnd, SW_SHOW);*/
			ShowWindow(hwnd, SW_HIDE);
			break;
		case ID_OK_BUTTON: //확占쏙옙 占쏙옙튼 클占쏙옙占쏙옙 - input 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占.
			

			GetDlgItemText(hwnd, ID_NOTICE_INPUT, input_notice_result, sizeof(input_notice_result));
			MessageBox(hwnd, input_notice_result, _T("占쏙옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙"), MB_OK); // 占쏙옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙
			BoradcaseSendToNotice(input_notice_result); //占쏙옙占쌜곤옙占쏙옙占쏙옙

			//ShowWindow(hwnd, SW_HIDE);	// 占쏙옙占쏙옙占쏙옙占쏙옙 占쌉뤄옙창 占쌥깍옙
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


//------------------------------------- 홈 占쏙옙占쏙옙占쏙옙占쏙옙 占쏙옙橘占싫 占쏙옙占쏙옙占쏙옙 占쏙옙占싸쏙옙占쏙옙 -----------------------------------------------------------------------//
// 홈 占쏙옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占싸쏙옙占쏙옙
LRESULT CALLBACK Home_PassWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CREATE:
		// 占싸깍옙占쏙옙 화占쏙옙 占십깍옙화 占쏙옙 占쏙옙트占쏙옙 占쏙옙占쏙옙
																								   //x,y,width,height
		CreateWindow(_T("STATIC"), _T("占쏙옙占쏙옙占쏙옙 占쏙옙橘占싫ｏ옙占 占쌉뤄옙占쏙옙占쌍쇽옙占쏙옙."), WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 100, 50, 400, 50, hwnd, NULL, NULL, NULL); // 占쏙옙占쏙옙占쏙옙 占쏙옙橘占싫 타占쏙옙틀
		CreateWindow(_T("EDIT"), _T("占쏙옙橘占싫 占쌉뤄옙占싹깍옙"), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 100, 150, 400, 50, hwnd, (HMENU)ID_PASSWORD_INPUT, NULL, NULL);	// 占쏙옙橘占싫 占쌉뤄옙 input
		CreateWindow(_T("BUTTON"), _T("확占쏙옙"), WS_VISIBLE | WS_CHILD, 180, 300, 120, 50, hwnd, (HMENU)ID_OK_BUTTON, NULL, NULL); // 확占쏙옙 占쏙옙튼
		CreateWindow(_T("BUTTON"), _T("占쏙옙占"), WS_VISIBLE | WS_CHILD, 320, 300, 120, 50, hwnd, (HMENU)ID_CANCLE_BUTTON, NULL, NULL);		// 占쏙옙占 占쏙옙튼
		break;

	case WM_COMMAND:
		// 占쏙옙튼 클占쏙옙 占싱븝옙트 처占쏙옙
		switch (LOWORD(wParam)) {
			// '占쏙옙占' 占쏙옙튼 클占쏙옙 처占쏙옙
		case ID_CANCLE_BUTTON:
			// 홈 占쏙옙占쏙옙占쏙옙占쏙옙 占쏙옙橘占싫 占쌉뤄옙창占쏙옙 占쏙옙占쏙옙占 占쏙옙占쏙옙 창占쏙옙 占쌕쏙옙 占쏙옙占싱곤옙 占쏙옙
		/*	ShowWindow(hwnd, SW_SHOW);*/
			ShowWindow(hwnd, SW_HIDE);
			break;
		case ID_OK_BUTTON: //확占쏙옙 占쏙옙튼 클占쏙옙占쏙옙 - input 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占.
			_TCHAR password[256]; // 占싱뱄옙 占쌍댐옙 占쏙옙占쏙옙占쏙옙占싱듸옙?
			_tcscpy(password, _T("abc123")); // Copy the string "abc123" into userId
			
			GetDlgItemText(hwnd, ID_PASSWORD_INPUT, input_result, sizeof(input_result));


			// 占쏙옙橘占싫ｏ옙占 占쏙옙치占싹댐옙占쏙옙 占쏙옙
			if (_tcscmp(password, input_result) == 0 )
			{
				CreateAndShowWindow_Home_Notice(hwndHome_Notice); // 占쏙옙占쏙옙占쏙옙占쏙옙 占쌉뤄옙 화占쏙옙 占쏙옙占쏙옙占쌍깍옙
				ShowWindow(hwnd, SW_HIDE);
			}
			else
			{
				MessageBox(hwndHome_Pass, _T("占쏙옙橘占싫ｏ옙占 占쏙옙치占쏙옙占쏙옙 占십쏙옙占싹댐옙. 占쌕쏙옙 占쌉뤄옙占쏙옙占쌍쇽옙占쏙옙."), _T("占쏙옙占쏙옙"), MB_OK);
				
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

//--------------------------------------------------지안----------------------------------------------------------------//
// 소켓 통신 스레드 함수 (0) - 로그인할때 소켓 통신하기
// 클라이언트와 데이터 통신
DWORD WINAPI LoginProcessClient()
{

	int retval;
	// socket()
	g_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (g_sock == INVALID_SOCKET) err_quit("socket()");
	
	// connect() : 기존 TCP 연결이 아닌 새로 만든것
	SOCKADDR_IN Loginserveraddr;
	ZeroMemory(&Loginserveraddr, sizeof(Loginserveraddr));

	Loginserveraddr.sin_family = AF_INET; //server에 대한 설정
	Loginserveraddr.sin_addr.s_addr = inet_addr(SERVERIP4_CHAR); //server에 대한 설정
	Loginserveraddr.sin_port = htons(SERVERPORT); //server에 대한 설정
	retval = connect(g_sock, (SOCKADDR*)&Loginserveraddr, sizeof(Loginserveraddr)); //연결할때, 서버소켓 정보를 준다. -> establishied 상태
	if (retval == SOCKET_ERROR) err_quit("connect()");

	
	
	// 데이터 통신에 사용할 변수
	//char buf[BUFSIZE + 1]="TCP연결해라잉";
	//int len;

	//retval = send(g_sock, buf, strlen(buf), 0); //버퍼사이즈로 보내기
	//if (retval == SOCKET_ERROR) {
	//	err_display("send()");
	//	//break;
	//}
	

	int len;
	len = sizeof(NICKNAME_CHAR);

	// 고정 크기 데이터 전송
	retval = sendn(g_sock, (char*)&NICKNAME_CHAR, BUFSIZE, 0, serveraddr, false);

	printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		//break;
	}

	//retval = sendn(g_sock, (char*)&len, sizeof(int), 0);
	//// 가변 크기 데이터 전송
	//retval = sendn(g_sock, (char*)&g_chatmsg, len, 0);
	if (retval == SOCKET_ERROR)
		return 0;

	return 0;
	/*
	//// 서버와 데이터 통신
	while (1) {
	//	// 데이터 보내기
		retval = send(g_sock, buf, strlen(buf), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			//break;
		}
		printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

		// 데이터 받기
		retval = recvn(g_sock, (char*)&buf, BUFSIZE, 0, serveraddr, false); //
		//retval = recvn(g_sock, buf, retval, 0); // retval를 다시 넣은 이유 : 내가 보낸만큼 다시 받기 위해서이다. (10byte보냈으면 10만큼 받게 N을 설정해준거)
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			//break;
		}
		else if (retval == 0)
			//break;

		// 받은 데이터 출력
		buf[retval] = '\0';
		printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", retval);
		printf("[받은 데이터] %s\n", buf);
	}

	//HANDLE hThread[2];

	//hThread[1] = CreateThread(NULL, 0, LoginProcessClient, (LPVOID)1, 0, NULL);
	//WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	// //윈속 종료
	//WSACleanup();
	*/

}


//----------------------------------------------------------------------------------------------------------------------//


// 占쏙옙占쏙옙 占쏙옙占 占쏙옙占쏙옙占쏙옙 占쌉쇽옙(1) - 占쏙옙占쏙옙
DWORD WINAPI ClientMain(LPVOID arg)
{
	int retval;

	if (g_isIPv6 == false && g_isUDP == false) { // TCP/IPv4 占쏙옙占쏙옙
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
	else if (g_isIPv6 == true && g_isUDP == false) { // TCP/IPv6 占쏙옙占쏙옙
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
	else if (g_isIPv6 == false && g_isUDP == true) { // UDP/IPv4 占쏙옙占쏙옙
		//========================================== 占쏙옙占쏙옙 ==========================================//
		
		//--------------------- UDP 占쏙옙占쏙옙 1 ----------------------//
		if (channel == CHANNEL_UDP1) { //UDP 채占쏙옙 1 占싱띰옙占
			MessageBox(NULL, _T("占쏙옙占쏙옙占싱곤옙 占쏙옙占쏙옙占쏙옙占쏙옙 UDP 채占쏙옙1 IPv4 클占쏙옙占싱억옙트 占쏙옙占쏙옙占쏙옙"), _T("占싯몌옙"), MB_ICONERROR);
			// socket()
			g_sock = socket(AF_INET, SOCK_DGRAM, 0);
			if (g_sock == INVALID_SOCKET) err_quit("socket()");

			// 占쏙옙티캐占쏙옙트 占쌓뤄옙 占쏙옙占쏙옙 - (UDP占쏙옙 占쏙옙占써설占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙占실뤄옙, connet() 占쏙옙占십울옙)
			struct ip_mreq mreq;
			mreq.imr_multiaddr.s_addr = inet_addr(SERVERIP4_CHAR_UDP1); // 占쏙옙占쏙옙占싹거놂옙 탈占쏙옙占쏙옙 IPv4 占쏙옙티占심쏙옙트 address(占쌍쇽옙) (占쏙옙占쏙옙占쏙옙 占쏙옙占싣몌옙)
			mreq.imr_interface.s_addr = htonl(INADDR_ANY);		// 占쏙옙占쏙옙 ip address (占쏙옙)
			retval = setsockopt(g_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
				(char*)&mreq, sizeof(mreq));
			if (retval == SOCKET_ERROR) err_quit("setsockopt()");

			// 占쏙옙占쏙옙 占쌍쇽옙 占쏙옙占쏙옙체 占십깍옙화
			ZeroMemory(&serveraddr, sizeof(serveraddr));
			serveraddr.sin_family = AF_INET;
			serveraddr.sin_addr.s_addr = inet_addr(SERVERIP4_CHAR_UDP1);
			serveraddr.sin_port = htons(SERVERPORT);


			// 데이터 통신에 사용할 변수
			char buf[BUFSIZE + 1] = "hello, I'am UDP JIAN. UDP Channel1 !!";
			int len;

			// 데이터 보내기
			retval = sendto(g_sock, buf, BUFSIZE, 0,
				(SOCKADDR*)&serveraddr, sizeof(serveraddr));
			if (retval == SOCKET_ERROR) {
				err_display("sendto()");
			}

			// 占쏙옙티캐占쏙옙트 占쌓뤄옙 탈占쏙옙 (占쏙옙占쏙옙占 udp 채占쏙옙 x占쌀띰옙 占싱뤄옙占쏙옙 占싹쇽옙)
			retval = setsockopt(g_sock, IPPROTO_IP, IP_DROP_MEMBERSHIP,
				(char*)&mreq, sizeof(mreq));
			if (retval == SOCKET_ERROR) err_quit("setsockopt()");
		}
		//------------------------------------------------------//

		//--------------------- UDP 占쏙옙占쏙옙 2 ----------------------//
		else if (channel == CHANNEL_UDP2) { //UDP 채占쏙옙 2占쏙옙占
			MessageBox(NULL, _T("占쏙옙占쏙옙占싱곤옙 占쏙옙占쏙옙占쏙옙占쏙옙 UDP 채占쏙옙2 IPv4 클占쏙옙占싱억옙트 占쏙옙占쏙옙占쏙옙"), _T("占싯몌옙"), MB_ICONERROR);
			// socket()
			g_sock = socket(AF_INET, SOCK_DGRAM, 0);
			if (g_sock == INVALID_SOCKET) err_quit("socket()");

			// 占쏙옙티캐占쏙옙트 占쌓뤄옙 占쏙옙占쏙옙 - (UDP占쏙옙 占쏙옙占써설占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙占실뤄옙, connet() 占쏙옙占십울옙)
			struct ip_mreq mreq;
			mreq.imr_multiaddr.s_addr = inet_addr(SERVERIP4_CHAR_UDP2); // 占쏙옙占쏙옙占싹거놂옙 탈占쏙옙占쏙옙 IPv4 占쏙옙티占심쏙옙트 address(占쌍쇽옙) (占쏙옙占쏙옙占쏙옙 占쏙옙占싣몌옙)
			mreq.imr_interface.s_addr = htonl(INADDR_ANY);		// 占쏙옙占쏙옙 ip address (占쏙옙)
			retval = setsockopt(g_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
				(char*)&mreq, sizeof(mreq));
			if (retval == SOCKET_ERROR) err_quit("setsockopt()");

			// 占쏙옙占쏙옙 占쌍쇽옙 占쏙옙占쏙옙체 占십깍옙화
			ZeroMemory(&serveraddr, sizeof(serveraddr));
			serveraddr.sin_family = AF_INET;
			serveraddr.sin_addr.s_addr = inet_addr(SERVERIP4_CHAR_UDP2);
			serveraddr.sin_port = htons(SERVERPORT);

			// 占쏙옙占쏙옙占쏙옙 占쏙옙탓占 占쏙옙占쏙옙占 占쏙옙占쏙옙
			char buf[BUFSIZE + 1] = "hello, I'am UDP JIAN. UDP Channel2 !!";
			int len;

			// 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙
			retval = sendto(g_sock, buf, strlen(buf), 0,
				(SOCKADDR*)&serveraddr, sizeof(serveraddr));
			if (retval == SOCKET_ERROR) {
				err_display("sendto()");
			}

			// 占쏙옙티캐占쏙옙트 占쌓뤄옙 탈占쏙옙 (占쏙옙占쏙옙占 udp 채占쏙옙 x占쌀띰옙 占싱뤄옙占쏙옙 占싹쇽옙)
			retval = setsockopt(g_sock, IPPROTO_IP, IP_DROP_MEMBERSHIP,
				(char*)&mreq, sizeof(mreq));
			if (retval == SOCKET_ERROR) err_quit("setsockopt()");

		}
		//------------------------------------------------------//
		
		//==========================================================================================//
	}
	else if (g_isIPv6 == true && g_isUDP == true) { // UDP/IPv6 占쏙옙占쏙옙
		MessageBox(NULL, _T("占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙 占십았쏙옙占싹댐옙."), _T("占싯몌옙"), MB_ICONERROR);
		exit(1);
	}
	MessageBox(NULL, _T("占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쌩쏙옙占싹댐옙."), _T("占싯몌옙"), MB_ICONINFORMATION);

	// 占싻깍옙 & 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙
	HANDLE hThread[2];
	hThread[0] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, WriteThread, NULL, 0, NULL);
	if (hThread[0] == NULL || hThread[1] == NULL) exit(1);
	g_bCommStarted = true;

	// 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占 (占쏙옙 占쏙옙 占싹놂옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙)
	retval = WaitForMultipleObjects(2, hThread, FALSE, INFINITE);
	retval -= WAIT_OBJECT_0;
	if (retval == 0)
		TerminateThread(hThread[1], 1);
	else
		TerminateThread(hThread[0], 1);
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);

	MessageBox(NULL, _T("占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙求占."), _T("占싯몌옙"), MB_ICONERROR);
	EnableWindow(g_hBtnSendFile, FALSE);
	EnableWindow(g_hBtnSendMsg, FALSE);
	EnableWindow(g_hBtnErasePic, FALSE);
	g_bCommStarted = false;
	closesocket(g_sock);
	return 0;
}

// 占쏙옙占쏙옙 占쏙옙占 占쏙옙占쏙옙占쏙옙 占쌉쇽옙(2) - 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙
DWORD WINAPI ReadThread(LPVOID arg)
{
	int retval;
	COMM_MSG comm_msg;
	CHAT_MSG* chat_msg;
	DRAWLINE_MSG* drawline_msg;
	ERASEPIC_MSG* erasepic_msg;
	char reciever[20], sender[20], tmp[5];


	// ------ 占쏙옙占쏙옙 --------
	char senderName[256];
	char recieverName[256];
	char sendMsg[256];
	char word[10];

	// ====== 占쏙옙호 ========
	DRAWELLIPSE_MSG* drawEllipse_msg;
	int serveraddrLen;
	int len;
	//

	while (1) {

		//// 占쏙옙占쏙옙占쏙옙 占쌨깍옙(占쏙옙占쏙옙 占쏙옙占쏙옙)
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

		//// 占쏙옙占쏙옙占쏙옙 占쌨깍옙(占쏙옙占쏙옙 占쏙옙占쏙옙)
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
		if (isMessageQueue == TRUE) {

		}

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
		// ============ 占쏙옙占쏙옙 ==========
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
				if (strcmp(sendMsg, word) == 0) {  // 占쏙옙占시어를 占쏙옙占쏙옙 占쏙옙占: 占쏙옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙構占 占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙

					DisplayText("[%s] 占쏙옙占쏙옙占쌉니댐옙!\r\n", word);
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
			// ============ 占쏙옙占쏙옙 ============
			g_serverDrawDetailInformation.width = drawline_msg->width;
			// ==============================
			g_serverDrawDetailInformation.color = drawline_msg->color;
			SendMessage(g_hDrawWnd, WM_DRAWLINE,
				MAKEWPARAM(drawline_msg->x0, drawline_msg->y0),
				MAKELPARAM(drawline_msg->x1, drawline_msg->y1));
			break;
		// ======== 占쏙옙호 ==========
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

// 占쏙옙占쏙옙 占쏙옙占 占쏙옙占쏙옙占쏙옙 占쌉쇽옙(3) - 占쏙옙占쏙옙占쏙옙 占쌜쏙옙
DWORD WINAPI WriteThread(LPVOID arg)
{
	int retval, len;
	char* nickName;

	// 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占
	while (1) {
		// 占쏙옙占쏙옙 占싹뤄옙 占쏙옙摸占쏙옙占
		WaitForSingleObject(g_hWriteEvent, INFINITE);
		// 占쏙옙占쌘울옙 占쏙옙占싱곤옙 0占싱몌옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙
		if (strlen(g_chatmsg.msg) == 0) {
			// [占쌨쏙옙占쏙옙 占쏙옙占쏙옙] 占쏙옙튼 활占쏙옙화
			EnableWindow(g_hBtnSendMsg, TRUE);
			// 占싻깍옙 占싹뤄옙 占싯몌옙占쏙옙
			SetEvent(g_hReadEvent);
			continue;
		}
		// ============ 占쏙옙호 ===========
		// 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙

		char sendMsg[256];
		if (g_chatmsg.type == TYPE_CHAT) {
			snprintf(sendMsg, sizeof(sendMsg), "{%s} %s", NICKNAME_CHAR, g_chatmsg.msg);
			strcpy(g_chatmsg.msg, sendMsg);

		}
		len = sizeof(g_chatmsg);

		// 占쏙옙占쏙옙 크占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙
		retval = sendn(g_sock, (char*)&g_chatmsg, BUFSIZE, 0, serveraddr, g_isUDP);

		//retval = sendn(g_sock, (char*)&len, sizeof(int), 0);
		//// 占쏙옙占쏙옙 크占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙
		//retval = sendn(g_sock, (char*)&g_chatmsg, len, 0);
		if (retval == SOCKET_ERROR) break;


		// [占쌨쏙옙占쏙옙 占쏙옙占쏙옙] 占쏙옙튼 활占쏙옙화
		EnableWindow(g_hBtnSendMsg, TRUE);
		// 占싻깍옙 占싹뤄옙 占싯몌옙占쏙옙
		SetEvent(g_hReadEvent);
	}
	return 0;
}

// 占쏙옙占쏙옙트 占쏙옙트占쏙옙 占쏙옙占 占쌉쇽옙
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
