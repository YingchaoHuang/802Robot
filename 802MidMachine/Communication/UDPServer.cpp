#include "UDPServer.h"
#include<iostream>

UDPServer::UDPServer(char*desIP,int desPort) {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData)!=0) {
		std::cout << "add winsock lib failed" << std::endl;
	};
	server = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	std::cout << "UDPServer Socket initialized" << std::endl;

	serverSockAddr.sin_family = AF_INET;
	serverSockAddr.sin_port = htons(desPort);
	serverSockAddr.sin_addr.s_addr = inet_addr(desIP);
	isRuning = true;
	//revDataThread = new std::thread(&UDPServer::RevData, this);
}

UDPServer::~UDPServer() {
	closesocket(server);
}

void UDPServer::SendMsg(const char* buf) const
{
	const int len = sizeof(buf);
	sendto(server, buf, len, 0, (SOCKADDR*)&serverSockAddr, sizeof(serverSockAddr));
}


void UDPServer::RevData() {

}