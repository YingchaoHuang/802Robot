#include "UDPClient.h"
#include<iostream>

UDPClient::UDPClient(int localPort) {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cout << "add winsock lib failed" << std::endl;
	};

	client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	std::cout << "Server Socketinitialized" << std::endl;
	this->localPort = localPort;
	clientSockAddr.sin_family = AF_INET;
	clientSockAddr.sin_port = htons(localPort);
	clientSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(client, (SOCKADDR*)&clientSockAddr, sizeof(clientSockAddr));
	isRuning = true;
	revDataThread = new std::thread(&UDPClient::RevData, this);

}

UDPClient::~UDPClient() {
	closesocket(client);
}

void UDPClient::RevData() const
{
	while (isRuning)
	{
		char buf[1024];
		memset(buf, 0, sizeof(buf));
		int clientSockAddrSize = sizeof(clientSockAddr);
		recvfrom(client, buf, sizeof(buf), 0, (SOCKADDR*)&clientSockAddr, &clientSockAddrSize);
		std::cout << buf << std::endl;
	}
}

