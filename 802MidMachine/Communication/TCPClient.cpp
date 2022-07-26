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
	while (isRuning)//尝试重新创建与连接（当服务端断开再启动后，期间需要保持尝试连接）
	{
		if (CreateSocketAndConnect())
		{
			char revSerData[200];
			memset(revSerData, 0, sizeof(revSerData));
			while (recv(clientSocket, revSerData, sizeof(revSerData), 0) > 0)//阻塞进入数据接收阶段,<0或==0情况均需要重新连接
			{
				printf("服务器发送的数据: %s\n", revSerData);
				//接收到服务端数据，TODO处理
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
	//发送数据
	int sendRes = send(clientSocket, buffer, (int)strlen(buffer), 0);
	if (sendRes == SOCKET_ERROR)
	{
		printf("客户端send()出现错误 : %d\n", WSAGetLastError());
		return false;
	}
	else
	{
		printf("客户端发送数据:%c 成功!\n", buffer);
		return true;
	}
}


BOOL TCPClient::CreateSocketAndConnect()
{
	//建立客户端socket
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET)
	{
		printf("套接字创建失败: %d  \n", WSAGetLastError());
		closesocket(clientSocket);
		clientSocket = NULL;
		return false;
	}
	//定义要连接的服务器地址
	sockaddr_in addrConServer;
	addrConServer.sin_family = AF_INET;
	addrConServer.sin_port = htons(8821);
	addrConServer.sin_addr.S_un.S_addr = inet_addr("192.168.1.11");
	if (connect(clientSocket, (SOCKADDR*)&addrConServer, sizeof(addrConServer)) == SOCKET_ERROR)
	{
		printf("客户端建立连接失败！\n");
		closesocket(clientSocket);
		clientSocket = NULL;
		return false;
	}
	else
	{
		printf("客户端建立连接成功，准备发送数据！\n");
		return true;
	}
}


bool TCPClient::isRunning() {
	return isRuning;
}