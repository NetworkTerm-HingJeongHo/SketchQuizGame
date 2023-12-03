#include "stdafx.h"

// // =========== 정호 =============
// 클라이언트 관리 배열
int nTotalSockets = 0;
int nTotalUDPSockets = 0;
SOCKETINFO* SocketInfoArray[FD_SETSIZE]; //TCP 유저들 있는 변수
SOCKADDR_IN UDPSocketInfoArray[FD_SETSIZE]; //UDP 유저들 있는 변수

SOCKET listen_sock4;
SOCKADDR_IN serveraddr;
SOCKET socket_UDP;

// ============= 연경 =============== 
//char* g_msgQueue[BUFSIZE];    // 메시지 원형 큐: 이전 대화내용 표시. 꽉 차면 가장 오래된 메시지부터 지워진다.
//int head = 0, tail = 0;           // 원형 큐 인덱스
MESSAGEQUEUE g_msgQueue;

int main(int argc, char* argv[])
{
	// ========= 정호 ========
	// 윈도우 클래스 등록
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = NULL;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = _T("MyWndClass");
	if (!RegisterClass(&wndclass)) return 1;

	// 임시 윈도우 생성
	HWND hWnd = CreateWindow(_T("MyWndClass"), _T("TCP 서버"), WS_OVERLAPPEDWINDOW,
		0, 0, 600, 400, NULL, NULL, NULL, NULL);
	if (hWnd == NULL) return 1;
	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	//HWND hDlg = (HWND)DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc);

	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	/*----- TCP/IPv4 소켓 초기화 시작 -----*/
	// 소켓 생성
	listen_sock4 = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock4 == INVALID_SOCKET) err_quit("socket()");

	// bind()
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock4, (struct sockaddr *)&serveraddr,
		sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock4, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// TCP/IPv4 소켓에 논블로킹 모드 설정
	u_long nonBlockingModeOn = 1;
	retval = ioctlsocket(listen_sock4, FIONBIO, &nonBlockingModeOn);
	if (retval == SOCKET_ERROR) {
		err_quit("ioctlsocket()");
	}
	/*----- TCP/IPv4 소켓 초기화 종료 -----*/

	/*----- TCP/IPv6 소켓 초기화 시작 -----*/
	// 소켓 생성
	SOCKET listen_sock6 = socket(AF_INET6, SOCK_STREAM, 0);
	if (listen_sock6 == INVALID_SOCKET) err_quit("socket()");

	// bind()
	struct sockaddr_in6 serveraddr6;
	memset(&serveraddr6, 0, sizeof(serveraddr6));
	serveraddr6.sin6_family = AF_INET6;
	serveraddr6.sin6_addr = in6addr_any;
	serveraddr6.sin6_port = htons(SERVERPORT);
	retval = bind(listen_sock6, (struct sockaddr *)&serveraddr6,
		sizeof(serveraddr6));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock6, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");
	/*----- TCP/IPv6 소켓 초기화 종료 -----*/

	/*----- UDP/IPv4 소켓 초기화 시작 -----*/
	// TODO: 소켓을 생성하고 초기화한다. == 정호 ==
	socket_UDP = socket(AF_INET, SOCK_DGRAM, 0);
	if (socket_UDP == INVALID_SOCKET)
	{
		err_quit("socket()");
	}

	retval = bind(socket_UDP, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		err_quit("bind()");
	}
	/*----- UDP/IPv4 소켓 초기화 종료 -----*/

	/*----- UDP/IPv6 소켓 초기화 시작 -----*/
	// TODO: 소켓을 생성하고 초기화한다.
	/*----- UDP/IPv6 소켓 초기화 종료 -----*/

	// 데이터 통신에 사용할 변수(공통)
	fd_set rset;
	SOCKET client_sock;
	int addrlen;
	// 데이터 통신에 사용할 변수(IPv4)
	struct sockaddr_in clientaddr4;
	// 데이터 통신에 사용할 변수(IPv6)
	struct sockaddr_in6 clientaddr6;
	
	// ========== 정호 ==========
	int recvLen; // 받은 가변 데이터 크기
	int sendLen; // 보낼 가변 데이터 크기

	// WSAAsyncSelect()

	// TCP는 연결을 해야하므로 FD_ACCEPT를 추가
	retval = WSAAsyncSelect(listen_sock4, hWnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE);
	if (retval == SOCKET_ERROR) err_quit("WSAAsyncSelect()");

	// UDP는 TCP와 달리 연결이 필요없으므로
	// FD_ACCEPT를 하지 않음.
	// FD_READ로 데이터를 수신할 수 있도록 설정
	retval = WSAAsyncSelect(socket_UDP, hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
	if (retval == SOCKET_ERROR) err_quit("WSAAsyncSelect()");

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// 윈속 종료
	WSACleanup();
	return 0;
}

// ======= 정호 =======
// 윈도우 메시지 처리
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_SOCKET: // 소켓 관련 윈도우 메시지
		ProcessSocketMessage(hWnd, uMsg, wParam, lParam);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// 소켓 관련 윈도우 메시지 처리
void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 데이터 통신에 사용할 변수
	SOCKETINFO* ptr;
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen, retval;
	char buf[BUFSIZE + 1];

	// 오류 발생 여부 확인
	if (WSAGETSELECTERROR(lParam)) {
		err_display(WSAGETSELECTERROR(lParam));
		RemoveSocketInfo(wParam);
		return;
	}

	// 메시지 처리
	switch (WSAGETSELECTEVENT(lParam)) {
	// 접속
	case FD_ACCEPT:
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock4, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			return;
		}
		else {
			// 접속한 클라이언트 정보 출력
			printf("\n[TCP/IPv4 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", 
				inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
			// 소켓 정보 추가
			AddSocketInfoTCP(client_sock);
			retval = WSAAsyncSelect(client_sock, hWnd,
				WM_SOCKET, FD_READ | FD_WRITE | FD_CLOSE);
			if (retval == SOCKET_ERROR) {
				err_display("WSAAsyncSelect()");
				RemoveSocketInfo(client_sock);
			}
		}
		break;
	case FD_READ:
		// TCP socket
		printf("FD_READ\n");
		if (wParam != socket_UDP)
		{
			ptr = GetSocketInfo(wParam);
			if (ptr->recvbytes > 0) {
				return;
			}
			// 고정 데이터 받기
			retval = recv(ptr->sock, ptr->buf, BUFSIZE, 0);
      // ======== 연경 ===========
			addMessage(ptr->buf);
      // =========================
      
			// ======== 지안 ==========//
			// 
			printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", retval);
			printf("[받은 데이터] %s\n", ptr->buf);
			// ======================//
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				RemoveSocketInfo(wParam);
				return;
			}
			printf("[TCP] 데이터 길이 : %d, 데이터 : %s\n", retval, buf);
			// 받은 바이트 수 누적
			ptr->recvbytes += retval;

			// 채팅 데이터만 표기한다.
		}
		// UDP socket
		else
		{
			// 데이터 받기
			addrlen = sizeof(clientaddr);
			retval = recvfrom(socket_UDP, buf, BUFSIZE, 0, (SOCKADDR*)&clientaddr, &addrlen);
      // ================== 연경 ============
			addMessage(buf);
      // ====================================
      
			printf("[UDP] 데이터 길이 : %d, 데이터 : %s\n", retval, buf);
			if (retval == SOCKET_ERROR) {
				err_display("recvfrom()");
				return;
			}

			// UDP로 접속한 클라 정보 수집
			AddSocketInfoUDP(clientaddr);
		}
	case FD_WRITE:
		// UDP 소켓이 아닌 경우 (TCP인 경우)
		if (wParam != socket_UDP)
		{
			ptr = GetSocketInfo(wParam);
			//for (int i = 0; i < nTotalSockets; i++) {
			//	SOCKETINFO* ptr = SocketInfoArray[i];
			if (ptr->recvbytes == BUFSIZE) {
				// 받은 바이트 수 리셋
				ptr->recvbytes = 0;

				// 현재 접속한 모든 클라이언트에게 데이터를 보냄!
				for (int j = 0; j < nTotalSockets; j++) {
					SOCKETINFO* ptr2 = SocketInfoArray[j];
					retval = send(ptr2->sock, ptr->buf, BUFSIZE, 0);
					if (retval == SOCKET_ERROR) {
						err_display("send()");
						RemoveSocketInfo(j);
						--j; // 루프 인덱스 보정
						continue;
					}
				}
			}
			//}
		}
		else
		{
			for (int i = 0; i < nTotalUDPSockets; i++)
			{
				SOCKADDR_IN clientUDP = UDPSocketInfoArray[i];
				// 데이터 보내기
				retval = sendto(socket_UDP, buf, BUFSIZE, 0, (SOCKADDR*)&clientUDP, sizeof(clientUDP));
				if (retval == SOCKET_ERROR) {
					err_display("sendto()");
					return;
				}
			}
		}
		break;
	case FD_CLOSE:
		RemoveSocketInfo(wParam);
		break;
	}
}

// 다이얼로그 프로시저
INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hIDListTabCtrl, hChatDataTabCtrl;
	TCITEM tItem_ID, tItem_ChatData;
	switch (uMsg)
	{
	case WM_INITDIALOG:
		hIDListTabCtrl = GetDlgItem(hDlg, IDC_IDLIST_TAB);
		tItem_ID.mask = TCIF_TEXT;
		tItem_ID.pszText = (LPTSTR)_T("전체");
		TabCtrl_InsertItem(hIDListTabCtrl, 0, &tItem_ID); // 첫 번째 탭 추가

		tItem_ID.pszText = (LPTSTR)_T("TCP 채널");
		TabCtrl_InsertItem(hIDListTabCtrl, 1, &tItem_ID); // 두 번째 탭 추가
		return TRUE;
	case WM_COMMAND:

	case WM_CLOSE:
		EndDialog(hDlg, 0); // 다이얼로그 상자 닫기
		return TRUE;
	}

	return FALSE;
}

// 소켓 정보 얻기
SOCKETINFO* GetSocketInfo(SOCKET sock)
{
	// 현재 접속한 클라이언트 중에서 일치하는 소켓 탐색
	for (int i = 0; i < nTotalSockets; i++)
	{
		SOCKETINFO* ptr = SocketInfoArray[i];
		// 찾았을 경우, 해당 소켓 반환
		if (ptr->sock == sock)
		{
			return ptr;
		}
	}
	return NULL;
}

// UDP 클라 정보 추가
bool AddSocketInfoUDP(SOCKADDR_IN addr)
{
	// 이전에 접속한 적이 있는 상태인지 확인
	for (int i = 0; i < nTotalUDPSockets; i++)
	{
		if (inet_ntoa(UDPSocketInfoArray[i].sin_addr) == inet_ntoa(addr.sin_addr) &&
			ntohs(UDPSocketInfoArray[i].sin_port) == ntohs(addr.sin_port) &&
			ntohs(UDPSocketInfoArray[i].sin_family) == ntohs(addr.sin_family))
		{
			return false;
		}
	}

	// UDP 클라 정보 추가
	UDPSocketInfoArray[nTotalUDPSockets++] = addr;
	return true;
}

// TCP 소켓 정보 추가
bool AddSocketInfoTCP(SOCKET sock)
{
	if (nTotalSockets >= FD_SETSIZE) {
		printf("[오류] 소켓 정보를 추가할 수 없습니다!\n");
		return false;
	}
	SOCKETINFO *ptr = new SOCKETINFO;
	if (ptr == NULL) {
		printf("[오류] 메모리가 부족합니다!\n");
		return false;
	}
	ptr->sock = sock;
	ptr->recvbytes = 0;


	// TCP 소켓 배열에 추가
	SocketInfoArray[nTotalSockets++] = ptr;

	return true;
}

// 소켓 정보 삭제
void RemoveSocketInfo(SOCKET sock)
{

	// 클라이언트 정보 얻기
	struct sockaddr_in clientaddr;
	int addrlen = sizeof(clientaddr);
	getpeername(sock, (struct sockaddr*)&clientaddr, &addrlen);
	// 클라이언트 정보 출력
	char addr[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
	printf("[TCP/IPv4 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		addr, ntohs(clientaddr.sin_port));

	// 클라이언트 소켓 제거
	for (int i = 0; i < nTotalSockets; i++)
	{
		SOCKETINFO* ptr = SocketInfoArray[i];
		// 찾았을 경우, 해당 소켓 반환
		if (ptr->sock == sock)
		{
			if (i != (nTotalSockets - 1))
			{
				SocketInfoArray[i] = SocketInfoArray[nTotalSockets - 1];
			}
			--nTotalSockets;
		}
	}

	// 소켓 닫기
	closesocket(sock);
}


void addMessage(char* message) {
	if ((g_msgQueue.tail + 1) % BUFSIZE == g_msgQueue.head) { //큐가 꽉찬 경우: 
		g_msgQueue.head = (g_msgQueue.head + 1) % BUFSIZE; //마지막 요소를 하나 지우고 공간 하나를 확보한다.
	}
	strcpy(g_msgQueue.queue[g_msgQueue.tail],message);
	g_msgQueue.tail = (g_msgQueue.tail + 1) % BUFSIZE;

}
