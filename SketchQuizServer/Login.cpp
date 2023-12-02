#include "stdafx.h"


// 소켓 ID (닉네임) 중복확인하는 함수. (
bool CheckIDDuplication(int nTotalSockets, SOCKETINFO* SocketInfoArray[], _TCHAR id[])
{							// 총 소켓 개수, 소켓 배열, 클라에게 받은 비교할 닉네임
	// 현재 접속한 클라이언트 중에서 일치하는 소켓 탐색
	for (int i = 0; i < nTotalSockets; i++)
	{
		SOCKETINFO* ptr = SocketInfoArray[i];
		
		// 중복을 찾았을 경우 -> false 반환 (아이디 쓰면안됨)
		if (_tcscmp(id, ptr->id_nickname) == 0) // 클라에게 받은 nickname과 소켓이 들고있는 id_nickname이 같으면
			//_tcscmp(userId, input_result) == 0
		{
			return false; // false를 반환한다.
		}
	}
	// 중복이 없는 경우 -> true 반환 (아이디 써도됨)
	
	_tcscmp(SocketInfoArray[nTotalSockets - 1] -> id_nickname, id); //A에 B 저장 (현재 내 소켓에 닉네임 저장)
	return true;
}

/*
// 소켓 정보 저장을 위한 구조체와 변수
typedef struct _socketinfo
{
	socket sock;
	char   buf[bufsize + 1];
	int    recvbytes;

	// ===== 연경 ======
	_tchar id_nickname[bufsize]; // 사용자 닉네임
	int    score = 0;       // 게임 플레이 점수
} socketinfo;

*/