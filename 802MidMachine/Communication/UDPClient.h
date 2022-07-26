#pragma once
#include<Winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)
#ifndef UDPCLIENT_H
#define UDPCLIENT_H
#include<thread>

class UDPClient
{
public:
	UDPClient(int localPort);
	~UDPClient();
	void RevData()const;

private:
	SOCKET client;
	int localPort;
	bool isRuning;
	sockaddr_in clientSockAddr;
	std::thread* revDataThread;
};
#endif // !