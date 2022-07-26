#include"TCPServer.h"
#include<iostream>

TCPServer::TCPServer() {
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0) {
		std::cout << "add winsock lib failed" << std::endl;
	}
	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socket==INVALID_SOCKET) {
		printf("socket create failed");
		closesocket(_socket);
		return;
	}

	sockaddr_in addrListen;
	addrListen.sin_family = AF_INET;
	addrListen.sin_port = htons(30001);
	addrListen.sin_addr.S_un.S_addr =INADDR_ANY;
	if (bind(_socket,(SOCKADDR*)&addrListen,sizeof(SOCKADDR))==SOCKET_ERROR) {
		printf("bind failed");
		closesocket(_socket);
		return;
	}

	if (listen(_socket,5)==SOCKET_ERROR) {
		printf("listen failed");
		closesocket(_socket);
		return;
	}
	isRuning = true;
	revDataThread = new std::thread(&TCPServer::RevData, this);
}

BOOL TCPServer::WaitClientConnect() {
	sockaddr_in remoteAddr;
	int remoteAddrLen = sizeof(remoteAddr);
	_revSocket = accept(_socket, (SOCKADDR*)&remoteAddr, &remoteAddrLen);
	if (_revSocket==INVALID_SOCKET) {
		printf("client send request,server accept failed:%s \n",WSAGetLastError());
		closesocket(_revSocket);
			return false;
	}
	else
	{
		/*printf("client connect to server:%s \n", inet_ntoa(remoteAddr.sin_addr));*/
		return true;
	}
}

void TCPServer::RevData() {
	byte length[4];
	byte type[4];
	while (isRuning)
	{
		if (WaitClientConnect()) {
			char revData[200];
			memset(revData, 0, sizeof(revData));
			while (recv(_revSocket, revData, 200, 0) > 0)
			{
				memcpy(data, revData, sizeof(revData));
				isReceived = true;
			}
		}
		closesocket(_revSocket);
		Sleep(1000);
	}
}

void TCPServer::SendMsg(char* msg,int size) {
	if (send(_revSocket, msg, size, 0) == SOCKET_ERROR) {
		//printf("server send error:%d \n", WSAGetLastError());
	}
	else
	{
		//byte* body1 = new byte[32];
		//			for (int i = 0; i < 32; i++) {
		//				body1[i] = msg[i + 8];
		//			}
		//			proto::responseSystemSetUp set1;
		//			set1.ParseFromArray(body1, 32);
		//			std::cout << "resinfo:" << set1.resinfo()<< std::endl;
		//std::cout << "server send :"  <<msg<< std::endl;
	}
}

bool TCPServer::isRunning() {
	return isRuning;
}

TCPServer::~TCPServer() {
	closesocket(_socket);
	closesocket(_revSocket);
}

int TCPServer::bytesToInt(byte* bytes)
{
	int addr = bytes[0] & 0xFF;
	addr |= ((bytes[1] << 8) & 0xFF00);
	addr |= ((bytes[2] << 16) & 0xFF0000);
	addr |= ((bytes[3] << 24) & 0xFF000000);
	return addr;
}

void TCPServer::merge(byte b1[], byte* b2, byte* b3, int length, byte* res) {
	memcpy(res, b1, 4);
	memcpy(res + 4, b2, 4);
	memcpy(res + 8, b3, length);
}

void TCPServer::merge(byte b1[], byte b2[], byte* res) {
	memcpy(res, b1, 4);
	memcpy(res + 4, b2, 4);
}

void TCPServer::intToByte(int u, byte b[]) {
	b[0] = (byte)(u);
	b[1] = (byte)(u >> 8);
	b[2] = (byte)(u >> 16);
	b[3] = (byte)(u >> 24);
}