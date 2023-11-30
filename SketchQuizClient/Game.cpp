#include "stdafx.h"


int roundNum = 0;   //진행한 문제 개수. 제시어 배열의 인덱스 역할도 한다.
int countdown = 30;
const _TCHAR* quizWord[4] = { _T("사과"), _T("바나나"), _T("포도"),_T("오렌지") };   // 제시어 배열
BOOL isGameOver = FALSE;
BOOL isOwner = FALSE;  // 문제를 내는 클라이언트일 경우 isOwner는 TRUE이다. 문제를 맞추는 사람인 경우 FALSE.

void gameStart(HWND statusTimer, HWND statusWord) {

	HANDLE hThread[2];
	hThread[0] = CreateThread(NULL, 0, TimerThread, (LPVOID)statusTimer, CREATE_SUSPENDED, NULL);
	hThread[1] = CreateThread(NULL, 0, GameThread, (LPVOID)statusWord, CREATE_SUSPENDED, NULL);

	//WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	
	//Display(statusTimer, "%d", "10");
	//Display(statusWord, "%s", "tmp");
	//Display(statusTimer, "%d", "10");
	//Display(statusWord, "%s", "tmp");
}

void Display(HWND g_status, const char* fmt, ...)
{

	va_list arg;
	va_start(arg, fmt);
	char cbuf[1024];
	vsprintf(cbuf, fmt, arg);
	va_end(arg);

	int nLength = GetWindowTextLength(g_status);
	SendMessage(g_status, EM_SETSEL, 0, 0);
	SendMessageA(g_status, EM_REPLACESEL, FALSE, (LPARAM)cbuf);
}

DWORD WINAPI TimerThread(LPVOID arg) {
	HWND status = (HWND)arg;
	clock_t start = clock();

	int timer = (int)start;
	countdown = 5 - timer;   //30부터 내려오는 카운트다운 형식
	char* timerText = NULL;

	while (!isGameOver) {
		if (countdown <= 0) {
			start = clock();
			newRound();

		}
		sprintf(timerText, "%d", countdown);
		//Display(status, "%d", timerText);
		SetDlgItemTextA(status, IDC_EDIT_TIMER, timerText);
	}

	return 0;
}

DWORD WINAPI GameThread(LPVOID arg) {

	HWND status = (HWND)arg;
	char* roundText = NULL;

	countdown = 30;

	while (!isGameOver) {
		//if (hThread == NULL) return 1;
		snprintf(roundText,sizeof(roundText), "%s", quizWord[roundNum]);
		//Display(status, "%s", roundText);

		SetDlgItemTextA(status, IDC_EDIT_WORD, roundText);
	}
	return 0;
}

//새 라운드가 시작될 때 실행
void newRound() {
	roundNum += 1;
	if (roundNum >= 1) {
		isGameOver = TRUE;
		return;
	}
	DisplayText("%d 번째 라운드 입니다.", roundNum+1);
	if (isOwner) {

		Display(g_hWordStatus, (const char*)quizWord[roundNum]);
	}

}



