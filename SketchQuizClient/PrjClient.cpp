#include "stdafx.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 이벤트 생성(각각 신호, 비신호 상태)
	g_hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if (g_hReadEvent == NULL) return 1;
	g_hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (g_hWriteEvent == NULL) return 1;

	// 전역 변수 초기화(일부)
	g_chatmsg.type = TYPE_CHAT;
	g_drawlinemsg.type = TYPE_DRAWLINE;
	g_drawlinemsg.color = RGB(255, 0, 0);
	g_erasepicmsg.type = TYPE_ERASEPIC;

	// ===== 정호 =====
	g_drawellipsemsg.type = TYPE_DRAWELLIPSE;
	g_drawellipsemsg.color = RGB(255, 0, 0);
	//

	g_hInstance = hInstance;


	//-------지안--------//
	// 홈 메인화면 윈도우 클래스 등록
	WNDCLASS wcMain = { 0 };
	wcMain.lpfnWndProc = HomeWndProc;
	wcMain.hInstance = hInstance;
	wcMain.lpszClassName = _T("MainWindowClass");
	RegisterClass(&wcMain);

	// 로그인 윈도우 클래스 등록
	WNDCLASS wcLogin = { 0 };
	wcLogin.lpfnWndProc = LoginWndProc;
	wcLogin.hInstance = hInstance;
	wcLogin.lpszClassName = _T("LoginWindowClass");
	RegisterClass(&wcLogin);

	//------------------//
	
	// 메인 윈도우(첫 화면) 생성
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, MainWndProc, 0, 0, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("MainWindow"), NULL };
	RegisterClassEx(&wcex);
	g_hMainWindow = CreateWindow(_T("MainWindow"), _T("Main Window"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 200, NULL, NULL, hInstance, NULL);

	ShowWindow(g_hMainWindow, nCmdShow);
	UpdateWindow(g_hMainWindow);

	// 메시지 루프
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!IsDialogMessage(g_hDialog, &msg)) // 대화 상자 메시지 처리
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// 이벤트 객체 제거
	CloseHandle(g_hReadEvent);
	CloseHandle(g_hWriteEvent);
	// 윈속 종료
	WSACleanup();
	return 0;
}

// 메인 윈도우 프로시저
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		// '그림판' 버튼 생성
		CreateWindow(_T("BUTTON"), _T("그림판"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 10, 10, 100, 30, hWnd, (HMENU)1, g_hInstance, NULL);

		// '로그인' 버튼 생성
		CreateWindow(_T("BUTTON"), _T("로그인"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 120, 10, 100, 30, hWnd, (HMENU)2, g_hInstance, NULL);

		// '랭킹' 버튼 생성 (세 번째로 위치)
		CreateWindow(_T("BUTTON"), _T("랭킹"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 230, 10, 100, 30, hWnd, (HMENU)4, g_hInstance, NULL);

		// '메인' 버튼 생성 (네 번째로 위치)
		CreateWindow(_T("BUTTON"), _T("메인"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 340, 10, 100, 30, hWnd, (HMENU)3, g_hInstance, NULL);
		break;
	}
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == 1) // '그림판' 버튼 클릭
		{
			if (!g_bDialogVisible) // 대화 상자가 현재 보이지 않는 경우
			{
				// 대화 상자를 만들고 표시하는 함수 호출
				CreateAndShowDialog(hWnd);
			}
			else
			{
				// 대화 상자가 이미 보이는 경우, 대화 상자를 활성화합니다.
				SetForegroundWindow(g_hDialog);
			}
			// 빈 윈도우창 숨기기
			ShowWindow(hWnd, SW_HIDE);
		}
		//---지안 ----//
		else if (LOWORD(wParam) == 2) // '로그인' 버튼 클릭
		{
			CreateAndShowWindow_Login(hwndLogin);
		}

		else if (LOWORD(wParam) == 3) // '메인' 버튼 클릭
		{
			CreateAndShowWindow_Home(hwndHome); // 메인 생성
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

// 대화상자 프로시저(그림판 창)
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hChkIsIPv6;
	static HWND hEditIPaddr;
	static HWND hEditPort;
	static HWND hChkIsUDP;
	static HWND hBtnConnect;
	static HWND hBtnSendFile; // 전역 변수에도 저장
	static HWND hBtnSendMsg; // 전역 변수에도 저장
	static HWND hEditMsg;
	static HWND hEditStatus; // 전역 변수에도 저장
	static HWND hColorRed;
	static HWND hColorGreen;
	static HWND hColorBlue;
	static HWND hBtnErasePic; // 전역 변수에도 저장
	static HWND hStaticDummy;

	static HWND hTimer;    // 타이머 표시 
	static HWND hQuiz;     // 제시어 표시

	//gameStart();

	switch (uMsg) {
	case WM_INITDIALOG:
		// 컨트롤 핸들 얻기
		hChkIsIPv6 = GetDlgItem(hDlg, IDC_ISIPV6);
		hEditIPaddr = GetDlgItem(hDlg, IDC_IPADDR);
		hEditPort = GetDlgItem(hDlg, IDC_PORT);
		hChkIsUDP = GetDlgItem(hDlg, IDC_ISUDP);
		hBtnConnect = GetDlgItem(hDlg, IDC_CONNECT);
		hBtnSendFile = GetDlgItem(hDlg, IDC_SENDFILE);
		g_hBtnSendFile = hBtnSendFile; // 전역 변수에 저장
		hBtnSendMsg = GetDlgItem(hDlg, IDC_SENDMSG);
		g_hBtnSendMsg = hBtnSendMsg; // 전역 변수에 저장
		hEditMsg = GetDlgItem(hDlg, IDC_MSG);
		hEditStatus = GetDlgItem(hDlg, IDC_STATUS);
		g_hEditStatus = hEditStatus; // 전역 변수에 저장
		hColorRed = GetDlgItem(hDlg, IDC_COLORRED);
		hColorGreen = GetDlgItem(hDlg, IDC_COLORGREEN);
		hColorBlue = GetDlgItem(hDlg, IDC_COLORBLUE);
		hBtnErasePic = GetDlgItem(hDlg, IDC_ERASEPIC);
		g_hBtnErasePic = hBtnErasePic; // 전역 변수에 저장
		hStaticDummy = GetDlgItem(hDlg, IDC_DUMMY);

		g_hTimerStatus = GetDlgItem(hDlg, IDC_TIMER);  // 타이머 표시하는 EditText 부분 가져오기
		g_hQuizStatus = GetDlgItem(hDlg, IDC_QUIZ);    // 제시어 표시하는 EditText 부분 가져오기

		// 컨트롤 초기화
		SetDlgItemText(hDlg, IDC_IPADDR, SERVERIP4);
		SetDlgItemInt(hDlg, IDC_PORT, SERVERPORT, FALSE);
		EnableWindow(g_hBtnSendFile, FALSE);
		EnableWindow(g_hBtnSendMsg, FALSE);
		SendMessage(hEditMsg, EM_SETLIMITTEXT, SIZE_DAT / 2, 0);
		SendMessage(hColorRed, BM_SETCHECK, BST_CHECKED, 0);
		SendMessage(hColorGreen, BM_SETCHECK, BST_UNCHECKED, 0);
		SendMessage(hColorBlue, BM_SETCHECK, BST_UNCHECKED, 0);
		EnableWindow(g_hBtnErasePic, FALSE);

		// 윈도우 클래스 등록
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

		// 자식 윈도우 생성
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
			// 컨트롤 상태 얻기
			GetDlgItemTextA(hDlg, IDC_IPADDR, g_ipaddr, sizeof(g_ipaddr));
			g_port = GetDlgItemInt(hDlg, IDC_PORT, NULL, TRUE);
			g_isIPv6 = SendMessage(hChkIsIPv6, BM_GETCHECK, 0, 0);
			g_isUDP = SendMessage(hChkIsUDP, BM_GETCHECK, 0, 0);
			// 소켓 통신 스레드 시작
			g_hClientThread = CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);
			if (g_hClientThread == NULL) exit(0);
			// 서버 접속 성공 기다림
			while (g_bCommStarted == false);
			// 컨트롤 상태 변경
			EnableWindow(hChkIsIPv6, FALSE);
			EnableWindow(hEditIPaddr, FALSE);
			EnableWindow(hEditPort, FALSE);
			EnableWindow(hChkIsUDP, FALSE);
			EnableWindow(hBtnConnect, FALSE);
			EnableWindow(g_hBtnSendFile, TRUE);
			EnableWindow(g_hBtnSendMsg, TRUE);
			SetFocus(hEditMsg);
			EnableWindow(g_hBtnErasePic, TRUE);
			return TRUE;
		case IDC_SENDFILE:
			MessageBox(NULL, _T("아직 구현하지 않았습니다."), _T("알림"), MB_ICONERROR);
			// 1. GetOpenFileName() 윈도우 API를 이용해서 파일 열기 대화상자를 열고
			// 전송할 파일을 선택한다. 2. 선택한 파일을 읽어서 서버에 전송한다.
			return TRUE;
		case IDC_SENDMSG:
			// 이전에 얻은 채팅 메시지 읽기 완료를 기다림
			WaitForSingleObject(g_hReadEvent, INFINITE);
			// 새로운 채팅 메시지를 얻고 쓰기 완료를 알림
			GetDlgItemTextA(hDlg, IDC_MSG, g_chatmsg.msg, SIZE_DAT);
			SetEvent(g_hWriteEvent);
			// 입력된 텍스트 전체를 선택 표시
			SendMessage(hEditMsg, EM_SETSEL, 0, -1);
			return TRUE;
		case IDC_COLORRED:
			g_drawlinemsg.color = RGB(255, 0, 0);
			return TRUE;
		case IDC_COLORGREEN:
			g_drawlinemsg.color = RGB(0, 255, 0);
			return TRUE;
		case IDC_COLORBLUE:
			g_drawlinemsg.color = RGB(0, 0, 255);
			return TRUE;
		case IDC_ERASEPIC:
			send(g_sock, (char*)&g_erasepicmsg, SIZE_TOT, 0);
			return TRUE;
		case IDCANCEL:
			//closesocket(g_sock);
			//EndDialog(hDlg, IDCANCEL);
			//ShowWindow(g_hDialog, SW_HIDE);
			CreateRankDlg(hDlg);

			return TRUE;
		}
	}
	return FALSE;
}

// 자식 윈도우 프로시저(그림판 영역)
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
		// 화면 출력용 DC 핸들 얻기
		hDC = GetDC(hWnd);
		// 배경 비트맵과 메모리 DC 생성
		cx = LOWORD(lParam);
		cy = HIWORD(lParam);
		hBitmap = CreateCompatibleBitmap(hDC, cx, cy);
		hDCMem = CreateCompatibleDC(hDC);
		SelectObject(hDCMem, hBitmap);
		// 배경 비트맵 흰색으로 채움
		SelectObject(hDCMem, GetStockObject(WHITE_BRUSH));
		SelectObject(hDCMem, GetStockObject(WHITE_PEN));
		Rectangle(hDCMem, 0, 0, cx, cy);
		// 화면 출력용 DC 핸들 해제
		ReleaseDC(hWnd, hDC);
		return 0;
	case WM_PAINT:
		// 화면 출력용 DC 핸들 얻기
		hDC = BeginPaint(hWnd, &ps);
		// 배경 비트맵을 화면에 전송
		BitBlt(hDC, 0, 0, cx, cy, hDCMem, 0, 0, SRCCOPY);
		// 화면 출력용 DC 핸들 해제
		EndPaint(hWnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		// 마우스 클릭 좌표 얻기
		x0 = LOWORD(lParam);
		y0 = HIWORD(lParam);
		bDrawing = true;
		return 0;
	case WM_MOUSEMOVE:
		if (bDrawing && g_bCommStarted) {
			// 마우스 클릭 좌표 얻기
			x1 = LOWORD(lParam);
			y1 = HIWORD(lParam);
			// 선 그리기 메시지 보내기
			g_drawlinemsg.x0 = x0;
			g_drawlinemsg.y0 = y0;
			g_drawlinemsg.x1 = x1;
			g_drawlinemsg.y1 = y1;
			send(g_sock, (char*)&g_drawlinemsg, SIZE_TOT, 0);
			// 마우스 클릭 좌표 갱신
			x0 = x1;
			y0 = y1;
		}
		return 0;
	case WM_LBUTTONUP:
		bDrawing = false;
		return 0;
	case WM_DRAWLINE:
		// 화면 출력용 DC와 Pen 핸들 얻기
		hDC = GetDC(hWnd);
		hPen = CreatePen(PS_SOLID, 3, g_drawcolor);
		// 윈도우 화면에 1차로 그리기
		hOldPen = (HPEN)SelectObject(hDC, hPen);
		MoveToEx(hDC, LOWORD(wParam), HIWORD(wParam), NULL);
		LineTo(hDC, LOWORD(lParam), HIWORD(lParam));
		SelectObject(hDC, hOldPen);
		// 배경 비트맵에 2차로 그리기
		hOldPen = (HPEN)SelectObject(hDCMem, hPen);
		MoveToEx(hDCMem, LOWORD(wParam), HIWORD(wParam), NULL);
		LineTo(hDCMem, LOWORD(lParam), HIWORD(lParam));
		SelectObject(hDCMem, hOldPen);
		// 화면 출력용 DC와 Pen 핸들 해제
		DeleteObject(hPen);
		ReleaseDC(hWnd, hDC);
		return 0;
	// ======== 정호 ==========
	case WM_DRAWELLIPSE:
		DrawEllipseProcess(hWnd, hDCMem, wParam, lParam, x0, y0);
		return 0;
	//
	case WM_ERASEPIC:
		// 배경 비트맵 흰색으로 채움
		SelectObject(hDCMem, GetStockObject(WHITE_BRUSH));
		SelectObject(hDCMem, GetStockObject(WHITE_PEN));
		Rectangle(hDCMem, 0, 0, cx, cy);
		// WM_PAINT 메시지 강제 생성
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

// ---- 지안 (로그인을 위함) ----- //
_TCHAR input_result[256]; // input 결과 저장할 배열
_TCHAR ID_NICKNAME[256]; // stdafx.h 파일에 같은 주소에 저장하기 위함

//-------------------------------//

// 로그인 윈도우 프로시저 (로그인 영역) -----------------------------------------------------------------------------------//
LRESULT CALLBACK LoginWndProc(HWND hwndLogin, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CREATE:
		// 로그인 화면 초기화 및 컨트롤 생성
																								   //x,y,width,height
		CreateWindow(_T("STATIC"), _T("스케치퀴즈"), WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 500, 100, 300, 100, hwndLogin, NULL, NULL, NULL); // 스케치퀴즈 타이틀
		CreateWindow(_T("EDIT"), _T(""), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 400, 300, 400, 40, hwndLogin, (HMENU)ID_ID_INPUT, NULL, NULL);
		CreateWindow(_T("BUTTON"), _T("중복확인"), WS_VISIBLE | WS_CHILD, 850, 300, 100, 40, hwndLogin, (HMENU)ID_DUPLICATION_BUTTON, NULL, NULL); // 중복확인 버튼
		//CreateWindow(_T("STATIC"), _T("사용 가능한 ID입니다!"), WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 400, 360, 400, 40, hwndLogin, NULL, NULL, NULL); // 사용 가능한 ID입니다!
		CreateWindow(_T("BUTTON"), _T("로그인"), WS_VISIBLE | WS_CHILD | WS_DISABLED, 400, 500, 500, 100, hwndLogin, (HMENU)ID_LOGIN_BUTTON, NULL, NULL); // 로그인 버튼 (처음 비활성화)
		CreateWindow(_T("BUTTON"), _T("돌아가기"), WS_VISIBLE | WS_CHILD, 100, 100, 100, 30, hwndLogin, (HMENU)ID_BACKHOME_BUTTON, NULL, NULL);
		break;

	case WM_COMMAND:
		// 버튼 클릭 이벤트 처리
		switch (LOWORD(wParam)) {
			// '돌아가기' 버튼 클릭 처리
		case ID_BACKHOME_BUTTON:
			// 로그인 창을 숨기고 메인창을 보이게 해야하는데 일단 로그인창 숨김
			ShowWindow(hwndLogin, SW_HIDE);
			break;

		case ID_LOGIN_BUTTON: // 로그인 버튼을 클릭했을 시

			_tcscpy(ID_NICKNAME, input_result); // 현재 입력한 ID 저장
			MessageBox(hwndLogin, ID_NICKNAME, _T("메인 화면으로 이동합니다."), MB_OK);

			CreateAndShowWindow_Home(hwndHome); // 메인 생성 및 보이게하기
			ShowWindow(hwndLogin, SW_HIDE);
			break;

		case ID_ID_INPUT: // ID를 입력해주세요! 입력했을 시
			break;

		case ID_DUPLICATION_BUTTON: //중복 확인 버튼 클릭했을 시, 중복 확인하기
			_TCHAR userId[256]; // 이미 있는 유저아이디?
			_tcscpy(userId, _T("abc123")); // Copy the string "abc123" into userId
			GetDlgItemText(hwndLogin, ID_ID_INPUT, input_result, sizeof(input_result));

			// 현재 있는 Id와, 입력한 아이디 와의 비교
			if (_tcscmp(userId, input_result) == 0 && _tcscmp(userId, _T("")))
			{
				MessageBox(hwndLogin, _T("이미 있는 아이디입니다. 다른 아이디를 사용해주세요."), _T("중복 확인 결과"), MB_OK);

				// 이미 있는 아이디인 경우 로그인 버튼 비활성화
				EnableWindow(GetDlgItem(hwndLogin, ID_LOGIN_BUTTON), FALSE);
			}
			else
			{
				MessageBox(hwndLogin, _T("사용 가능한 아이디입니다."), _T("중복 확인 결과"), MB_OK);

				// 사용 가능한 아이디인 경우 로그인 버튼 활성화
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


//-------------------------------------홈 윈도우 프로시저 -----------------------------------------------------------------------//
// 윈도우 프로시저
LRESULT CALLBACK HomeWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CREATE:
		// 홈 메인 화면 초기화 및 컨트롤 생성
																							   //x,y,width,height
		CreateWindow(_T("STATIC"), ID_NICKNAME, WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 900, 10, 100, 30, hwnd, NULL, NULL, NULL); // 유저 id 출력
		CreateWindow(_T("STATIC"), _T("님 반갑습니다!"), WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 1000, 10, 200, 30, hwnd, NULL, NULL, NULL); // id 님 반갑습니다!
		CreateWindow(_T("STATIC"), _T("공자사항 내용"), WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 50, 50, 1150, 100, hwnd, NULL, NULL, NULL); // 스케치퀴즈 타이틀

		CreateWindow(_T("BUTTON"), _T("공지 전송"), WS_VISIBLE | WS_CHILD, 1042, 185, 174, 54, hwnd, (HMENU)ID_BACKHOME_BUTTON, NULL, NULL); // 공지 전송

		CreateWindow(_T("BUTTON"), _T("채널 A 입장"), WS_VISIBLE | WS_CHILD, 300, 200, 640, 100, hwnd, (HMENU)ID_CHANNEL_A_BUTTON, NULL, NULL); // 채널 A 입장
		CreateWindow(_T("BUTTON"), _T("채널 B 입장"), WS_VISIBLE | WS_CHILD, 300, 350, 640, 100, hwnd, (HMENU)ID_CHANNEL_B_BUTTON, NULL, NULL); // 채널 B 입장

		//CreateWindow(L"BUTTON", L"방만들기", WS_VISIBLE | WS_CHILD, 282, 600, 320, 67, hwnd, (HMENU)ID_BACKHOME_BUTTON, NULL, NULL); // 방 만들기
		CreateWindow(_T("BUTTON"), _T("랜덤입장"), WS_VISIBLE | WS_CHILD, 300, 500, 640, 100, hwnd, (HMENU)ID_CHANNEL_RANDOM_BUTTON, NULL, NULL); // 랜덤 입장


		CreateWindow(_T("BUTTON"), _T("돌아가기"), WS_VISIBLE | WS_CHILD, 100, 100, 100, 30, hwnd, (HMENU)ID_BACKHOME_BUTTON, NULL, NULL); // 돌아가기
		break;

	case WM_COMMAND:
		// 버튼 클릭 이벤트 처리
		switch (LOWORD(wParam)) {
			// '돌아가기' 버튼 클릭 처리
		case ID_BACKHOME_BUTTON:
			// 로그인 창을 숨기고 메인 창을 다시 보이게 함
		/*	ShowWindow(hwnd, SW_SHOW);*/
			ShowWindow(hwnd, SW_HIDE);
			break;

		case ID_CHANNEL_A_BUTTON: // 채널 A 버튼 클릭시
			CreateAndShowDialog(hwnd);
			break;
		case ID_CHANNEL_B_BUTTON: // 채널 B 버튼 클릭시
			CreateAndShowDialog(hwnd);
			break;
		case ID_CHANNEL_RANDOM_BUTTON: // 채널 랜덤 버튼 클릭시
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


// 소켓 통신 스레드 함수(1) - 메인
DWORD WINAPI ClientMain(LPVOID arg)
{
	int retval;

	if (g_isIPv6 == false && g_isUDP == false) { // TCP/IPv4 서버
		// socket()
		g_sock = socket(AF_INET, SOCK_STREAM, 0);
		if (g_sock == INVALID_SOCKET) err_quit("socket()");

		// connect()
		struct sockaddr_in serveraddr;
		memset(&serveraddr, 0, sizeof(serveraddr));
		serveraddr.sin_family = AF_INET;
		inet_pton(AF_INET, g_ipaddr, &serveraddr.sin_addr);
		serveraddr.sin_port = htons(g_port);
		retval = connect(g_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
		if (retval == SOCKET_ERROR) err_quit("connect()");
	}
	else if (g_isIPv6 == true && g_isUDP == false) { // TCP/IPv6 서버
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
	else if (g_isIPv6 == false && g_isUDP == true) { // UDP/IPv4 서버
		MessageBox(NULL, _T("아직 구현하지 않았습니다."), _T("알림"), MB_ICONERROR);
		exit(1);
	}
	else if (g_isIPv6 == true && g_isUDP == true) { // UDP/IPv6 서버
		MessageBox(NULL, _T("아직 구현하지 않았습니다."), _T("알림"), MB_ICONERROR);
		exit(1);
	}
	MessageBox(NULL, _T("서버에 접속했습니다."), _T("알림"), MB_ICONINFORMATION);

	// 읽기 & 쓰기 스레드 생성
	HANDLE hThread[2];
	hThread[0] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, WriteThread, NULL, 0, NULL);
	if (hThread[0] == NULL || hThread[1] == NULL) exit(1);
	g_bCommStarted = true;

	// 스레드 종료 대기 (둘 중 하나라도 종료할 때까지)
	retval = WaitForMultipleObjects(2, hThread, FALSE, INFINITE);
	retval -= WAIT_OBJECT_0;
	if (retval == 0)
		TerminateThread(hThread[1], 1);
	else
		TerminateThread(hThread[0], 1);
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);

	MessageBox(NULL, _T("연결이 끊겼습니다."), _T("알림"), MB_ICONERROR);
	EnableWindow(g_hBtnSendFile, FALSE);
	EnableWindow(g_hBtnSendMsg, FALSE);
	EnableWindow(g_hBtnErasePic, FALSE);
	g_bCommStarted = false;
	closesocket(g_sock);
	return 0;
}

// 소켓 통신 스레드 함수(2) - 데이터 수신
DWORD WINAPI ReadThread(LPVOID arg)
{
	int retval;
	COMM_MSG comm_msg;
	CHAT_MSG* chat_msg;
	DRAWLINE_MSG* drawline_msg;
	ERASEPIC_MSG* erasepic_msg;
	char reciever[20], sender[20], tmp[5];

	while (1) {
		retval = recv(g_sock, (char*)&comm_msg, SIZE_TOT, MSG_WAITALL);
		if (retval == 0 || retval == SOCKET_ERROR) {
			break;
		}
		if (comm_msg.type == TYPE_CHAT) {
			chat_msg = (CHAT_MSG*)&comm_msg;
			DisplayText("[받은 메시지] %s\r\n", chat_msg->msg);
			if (strncmp(chat_msg->msg, "/w ", 2) == 0) {
				sscanf(chat_msg->msg, "%s %s %s", tmp, sender, reciever);
				MySendFile(sender, reciever, chat_msg->msg);
				
			}
		}
		else if (comm_msg.type == TYPE_DRAWLINE) {
			drawline_msg = (DRAWLINE_MSG*)&comm_msg;
			g_drawcolor = drawline_msg->color;
			SendMessage(g_hDrawWnd, WM_DRAWLINE,
				MAKEWPARAM(drawline_msg->x0, drawline_msg->y0),
				MAKELPARAM(drawline_msg->x1, drawline_msg->y1));
		}
		else if (comm_msg.type == TYPE_ERASEPIC) {
			erasepic_msg = (ERASEPIC_MSG*)&comm_msg;
			SendMessage(g_hDrawWnd, WM_ERASEPIC, 0, 0);
		}
	}
	return 0;
}

// 소켓 통신 스레드 함수(3) - 데이터 송신
DWORD WINAPI WriteThread(LPVOID arg)
{
	int retval;

	// 서버와 데이터 통신
	while (1) {
		// 쓰기 완료 기다리기
		WaitForSingleObject(g_hWriteEvent, INFINITE);
		// 문자열 길이가 0이면 보내지 않음
		if (strlen(g_chatmsg.msg) == 0) {
			// [메시지 전송] 버튼 활성화
			EnableWindow(g_hBtnSendMsg, TRUE);
			// 읽기 완료 알리기
			SetEvent(g_hReadEvent);
			continue;
		}
		// 데이터 보내기
		retval = send(g_sock, (char*)&g_chatmsg, SIZE_TOT, 0);
		if (retval == SOCKET_ERROR) break;
		// [메시지 전송] 버튼 활성화
		EnableWindow(g_hBtnSendMsg, TRUE);
		// 읽기 완료 알리기
		SetEvent(g_hReadEvent);
	}
	return 0;
}

// 에디트 컨트롤 출력 함수
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
