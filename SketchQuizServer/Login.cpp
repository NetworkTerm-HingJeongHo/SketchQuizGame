#include "stdafx.h"


// ���� ID (�г���) �ߺ�Ȯ���ϴ� �Լ�. (
bool CheckIDDuplication(int nTotalSockets, SOCKETINFO* SocketInfoArray[], _TCHAR id[])
{							// �� ���� ����, ���� �迭, Ŭ�󿡰� ���� ���� �г���
	// ���� ������ Ŭ���̾�Ʈ �߿��� ��ġ�ϴ� ���� Ž��
	for (int i = 0; i < nTotalSockets; i++)
	{
		SOCKETINFO* ptr = SocketInfoArray[i];
		
		// �ߺ��� ã���� ��� -> false ��ȯ (���̵� ����ȵ�)
		if (_tcscmp(id, ptr->id_nickname) == 0) // Ŭ�󿡰� ���� nickname�� ������ ����ִ� id_nickname�� ������
			//_tcscmp(userId, input_result) == 0
		{
			return false; // false�� ��ȯ�Ѵ�.
		}
	}
	// �ߺ��� ���� ��� -> true ��ȯ (���̵� �ᵵ��)
	
	_tcscmp(SocketInfoArray[nTotalSockets - 1] -> id_nickname, id); //A�� B ���� (���� �� ���Ͽ� �г��� ����)
	return true;
}

/*
// ���� ���� ������ ���� ����ü�� ����
typedef struct _socketinfo
{
	socket sock;
	char   buf[bufsize + 1];
	int    recvbytes;

	// ===== ���� ======
	_tchar id_nickname[bufsize]; // ����� �г���
	int    score = 0;       // ���� �÷��� ����
} socketinfo;

*/