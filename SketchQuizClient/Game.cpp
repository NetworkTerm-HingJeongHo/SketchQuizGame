#include "stdafx.h"


int roundNum = 0;   //������ ���� ����. ���þ� �迭�� �ε��� ���ҵ� �Ѵ�.
int countdown = 30;
const TCHAR* quizWord[4] = { _T("���"), _T("�ٳ���"), _T("����"),_T("������") };   // ���þ� �迭
BOOL isGameOver = FALSE;
BOOL isOwner = FALSE;  // ������ ���� Ŭ���̾�Ʈ�� ��� isOwner�� TRUE�̴�. ������ ���ߴ� ����� ��� FALSE.

void gameStart() {

	//HANDLE hThread[2];
	//hThread[0] = CreateThread(NULL, 0, TimerThread, NULL, CREATE_SUSPENDED, NULL);
	//hThread[1] = CreateThread(NULL, 0, GameThread, NULL, CREATE_SUSPENDED, NULL);

	//WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	
	Display(g_hTimerStatus, "%d", "10");
	Display(g_hWordStatus, "%s", "tmp");

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

	clock_t start = clock();

	int timer = (int)start;
	countdown = 30 - timer;   //30���� �������� ī��Ʈ�ٿ� ����
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

DWORD WINAPI GameThread(LPVOID arg) {
	int myScore = 0;
	//quizWord = { (char*)"���", (char*)"�ٳ���", (char*)"����", (char*)"������" };
	char* roundText = NULL;

	countdown = 30;

	while (!isGameOver) {
		//if (hThread == NULL) return 1;
		sprintf(roundText, "%d", roundNum);
		Display(g_hWordStatus, roundText);
	}
	return 0;
}

//�� ���尡 ���۵� �� ����
void newRound() {
	roundNum += 1;
	if (roundNum >= 1) {
		isGameOver = TRUE;
		return;
	}
	DisplayText("%d ��° ���� �Դϴ�.", roundNum+1);
	if (isOwner) {

		Display(g_hWordStatus, (const char*)quizWord[roundNum]);
	}

}


