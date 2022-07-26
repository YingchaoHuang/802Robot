#include"TCPClient.h"
#include<iostream>

TCPClient::TCPClient() {
	WORD sockVerson = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVerson, &wsaData) != 0)
	{
		std::cout << "add winsock lib failed" << std::endl;
	}
	isRuning = true;
	revDataThread = new std::thread(&TCPClient::RevData, this);
}

TCPClient::~TCPClient() {
	closesocket(clientSocket);
}

void TCPClient::RevData()
{
	while (isRuning)//�������´��������ӣ�������˶Ͽ����������ڼ���Ҫ���ֳ������ӣ�
	{
		if (CreateSocketAndConnect())
		{
			char revSerData[200];
			memset(revSerData, 0, sizeof(revSerData));
			while (recv(clientSocket, revSerData, sizeof(revSerData), 0) > 0)//�����������ݽ��ս׶�,<0��==0�������Ҫ��������
			{
				printf("���������͵�����: %s\n", revSerData);
				//���յ���������ݣ�TODO����
			}
		}
		else
		{
			Sleep(1000);
			continue;
		}
		closesocket(clientSocket);
		clientSocket = NULL;
		Sleep(1000);
	}
}



bool TCPClient::SendMsg(char* buffer)
{
	//��������
	int sendRes = send(clientSocket, buffer, (int)strlen(buffer), 0);
	if (sendRes == SOCKET_ERROR)
	{
		printf("�ͻ���send()���ִ��� : %d\n", WSAGetLastError());
		return false;
	}
	else
	{
		printf("�ͻ��˷�������:%c �ɹ�!\n", buffer);
		return true;
	}
}


BOOL TCPClient::CreateSocketAndConnect()
{
	//�����ͻ���socket
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET)
	{
		printf("�׽��ִ���ʧ��: %d  \n", WSAGetLastError());
		closesocket(clientSocket);
		clientSocket = NULL;
		return false;
	}
	//����Ҫ���ӵķ�������ַ
	sockaddr_in addrConServer;
	addrConServer.sin_family = AF_INET;
	addrConServer.sin_port = htons(8821);
	addrConServer.sin_addr.S_un.S_addr = inet_addr("192.168.1.11");
	if (connect(clientSocket, (SOCKADDR*)&addrConServer, sizeof(addrConServer)) == SOCKET_ERROR)
	{
		printf("�ͻ��˽�������ʧ�ܣ�\n");
		closesocket(clientSocket);
		clientSocket = NULL;
		return false;
	}
	else
	{
		printf("�ͻ��˽������ӳɹ���׼���������ݣ�\n");
		return true;
	}
}


bool TCPClient::isRunning() {
	return isRuning;
}