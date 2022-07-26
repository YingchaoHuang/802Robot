#pragma once
#pragma comment(lib, "Ws2_32.lib")
#ifndef TCPSERVER_H
#define TCPSERVER_H
#include<Winsock2.h>
#include<thread>
#include<string>
#include<string>

constexpr auto SERVER_PORT = 30001;


class TCPServer
{
public:
	TCPServer();
	~TCPServer();
	void SendMsg(char* msg,int size);
	bool isRunning();
	BOOL WaitClientConnect();
	void RevData();
	std::string revMsg;
	char data[200];
	bool isReceived;
private:
	SOCKET _socket;
	SOCKET _revSocket;
	bool isRuning;
	std::thread*revDataThread;
	enum {
		DATA_BUFFER_LEN = 2097152, HEADER_LEN = 8, CMD_TYPE_LEN = 4
	};
	int bytesToInt(byte* bytes);
	void intToByte(int u, byte b[]);
	void merge(byte b1[], byte* b2, byte* b3, int length, byte* res);
	void merge(byte b1[], byte b2[], byte* res);
};



#endif // !


