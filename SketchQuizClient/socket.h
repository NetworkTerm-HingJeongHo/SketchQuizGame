#pragma once

// ���ϰ� ���õ� ���

// =========== ��ȣ ===========

// ���� ũ�� : �ش� byte��ŭ �ޱ�
int recvn(SOCKET s, char* buf, int len, int flags, SOCKADDR_IN serveraddr, bool bIsUDP);

// ���� ũ�� : �ش� byte��ŭ ������
int sendn(SOCKET s, char* buf, int len, int flags, SOCKADDR_IN serveraddr, bool bIsUDP);

// ���� ũ�� : ���� ũ�⸦ �˷��ִ� �޽��� ����
void sendMsgLen(SOCKET s, int len, SOCKADDR_IN serveraddr, bool bIsUDP);