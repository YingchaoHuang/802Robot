#pragma once
#include<Winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)
#ifndef UDPSERVER_H
#define UDPSERVER_H
#include<thread>

class UDPServer
{
public:
	UDPServer(char*desIP,int desPort);
	~UDPServer();
	void RevData();
	void SendMsg(const char* msg)const;

private:
	SOCKET server;
	int desPort;
	char* desIP;
	bool isRuning;
	sockaddr_in serverSockAddr;
	std::thread* revDataThread;
};

#endif // !