#include "stdafx.h"

void MySendFile(char* senderName, char* reciverName, char* msg) {

	FILE* fp;
	//char* fileName, *tmp;
	//fileName = strcat((char*)"FROM_", senderName);
	//tmp = strcat((char*)"_TO_", reciverName);
	//fileName = strcat(fileName, tmp);
	//fileName = strcat(fileName, (char*)"txt");

	if ((fp = fopen("test.txt", "a")) == NULL) {
		printf("���� �б� ����! \n");
	}

	fwrite(msg, sizeof(char*), sizeof(msg), fp);

	int retval;

	// ������ ��ſ� ����� ����
	char buf[BUFSIZE];

	int len;
	while (len = fread(buf, sizeof(char), BUFSIZE, fp)) {   // ���� ������ �д´�
		// BUFSIZE��ŭ ������ �о�鿩 server�� ������ ���� �� ���� while ��ƾ�̴�.
		// 
		// ������ ������(���� ����)
		retval = send(g_sock, (char*)&len, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}

		// ������ ������(���� ����)
		retval = send(g_sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
		printf("[TCP Ŭ���̾�Ʈ] %d+%d����Ʈ�� "
			"���½��ϴ�.\n", sizeof(int), retval);
	}

	fclose(fp);
}
