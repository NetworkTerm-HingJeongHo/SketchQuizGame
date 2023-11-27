#include "stdafx.h"


int roundNum = 0;   //진행한 문제 개수. 제시어 배열의 인덱스 역할도 한다.
int countdown = 30;
const TCHAR* quizWord[4] = { _T("사과"), _T("바나나"), _T("포도"),_T("오렌지") };   // 제시어 배열
BOOL isGameOver = FALSE;
BOOL isOwner = FALSE;  // 문제를 내는 클라이언트일 경우 isOwner는 TRUE이다. 문제를 맞추는 사람인 경우 FALSE.

void gameStart() {
	int myScore = 0;
	//quizWord = { (char*)"사과", (char*)"바나나", (char*)"포도", (char*)"오렌지" };
	char* roundText = NULL;

	countdown = 30;

	while (!isGameOver) {

		sprintf(roundText, "%d", roundNum);
		Display(g_hQuizStatus, roundText);
	}
	
	//itoa(timer, timerText, 10);
	//itoa(roundNum, roundText, 10);

}

void Display(HWND g_status, const char* fmt, ...)
{

	va_list arg;
	va_start(arg, fmt);
	char cbuf[1024];
	vsprintf(cbuf, fmt, arg);
	va_end(arg);

	int nLength = GetWindowTextLength(g_status);
	SendMessage(g_status, EM_SETSEL, nLength, nLength);
	SendMessageA(g_status, EM_REPLACESEL, FALSE, (LPARAM)cbuf);
}

DWORD WINAPI TimerThread(LPVOID arg) {

	clock_t start = clock();

	int timer = (int)start;
	countdown = 30 - timer;   //30부터 내려오는 카운트다운 형식
	char* timerText = NULL;

	while (!isGameOver) {
		if (countdown <= 0) {
			start = clock();
			newRound();

		}
		sprintf(timerText, "%d", countdown);
		Display(g_hTimerStatus, timerText);
	}

	return 0;
}

//새 라운드가 시작될 때 실행
void newRound() {
	roundNum += 1;
	DisplayText("%d 번째 라운드 입니다.", roundNum+1);
	if (isOwner) {

		Display(g_hQuizStatus, (const char*)quizWord[roundNum]);
	}
}



