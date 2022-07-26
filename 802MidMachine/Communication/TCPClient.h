#pragma once
#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4996)
#ifndef TCPCLIENT_H
#define TCPCLIENT_H
#include<Winsock2.h>
#include<thread>

class TCPClient
{
public:
	TCPClient();
	~TCPClient();
	bool SendMsg(char* buffers);
	bool isRunning();
	BOOL CreateSocketAndConnect();
	void RevData();

private:
	SOCKET clientSocket;
	bool isRuning;
	std::thread* revDataThread;
};
#endif // !