#include <exception>
#include <iostream>
#include <Windows.h>
#include <stdlib.h>
#include "ITCPClient.h"
#include "ITCPServer.h"
#include "ITCPRemoteClient.h"
#include "WindowsTCPClient.h"
#include "myTCPServer.h"



void		sampleServerTest()
{
	std::string	ip = "";
	ITCPServer	*server;
	server = new myTCPServer();
	server->start(ip, 4340, 10);
	std::cout << "Server started" << std::endl;

	ITCPRemoteClient *acctedClient = server->acceptClient();
	std::cout << "Accepting new client" << std::endl;

	system("PAUSE");
}

void		basicTest()
{
	int				size;
	std::string		dest;
	std::string ip = "127.0.0.1";
	unsigned short port = 4242;
	ITCPClient			*client;
	ITCPRemoteClient	*rClient;
	ITCPServer			*server;

	server = new WindowsTCPServer();
	server->start(ip, port, 10);
	std::cout << "Launching server ..." << std::endl;
	Sleep(2);
	std::cout << "Creating new client ...." << std::endl;
	client = new WindowsTCPClient();
	Sleep(1);
	std::cout << "Connecting to server ... " << std::endl;
	if (client->connectTo(ip, port) == -1)
		throw std::runtime_error("Cannot connect to server");
	Sleep(1);

	rClient = server->acceptClient();
	std::cout << "Connected to server ..." << std::endl,
	client->prepareMsg("HELLO", 6);
	size = client->send();
	std::cout << "CLIENT : Send HELLO ... ( size = " << size << ")" << std::endl;
	Sleep(1);
	size = (*rClient >> dest);
	std::cout << "SERVER : Receive : " << dest << " ( size = " << size << ")" << std::endl;
	Sleep(1);

	rClient->prepareMsg("HOW ARE U ?", 12);
	size = rClient->send();
	std::cout << "SERVER : Send HOW ARE U ? ... ( size = " << size << ")" << std::endl;
	Sleep(1);
	
	dest = "";
	size = (*client >> dest);
	std::cout << "CLIENT : Receive :" << dest << " (size = " << size << ")" << std::endl;
	Sleep(1);

	std::cout << "Disconnecting client ... " << std::endl;
	client->closeSock();
	delete client;

	dest = "";
	size = (*rClient >> dest);
	std::cout << "Server : Receive :" << dest << " (size = " << size << ")" << std::endl;
	Sleep(1);
	delete rClient;
	server->stop();
	delete server;

	system("PAUSE");
}

int main()
{
	basicTest();
}
