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
	std::string ip = "127.0.0.1";
	unsigned short port = 4242;
	ITCPClient  *client;
	ITCPClient	*client2;
	ITCPServer	*server;

	server = new WindowsTCPServer();
	server->start(ip, port, 10);
	std::cout << "Launching server ..." << std::endl;
	Sleep(2);
	std::cout << "Creating new client ...." << std::endl;
	client = new WindowsTCPClient();
	Sleep(1);
	std::cout << "Connecting to server ... " << std::endl;
	client->connectTo(ip, port);
	Sleep(1);
	std::cout << "CLIENT : Sending HELLO ..." << std::endl;
	client->prepareMsg("HELLO", 6);
	client->send();
	Sleep(1);
	std::cout << "CLIENT : SENDING HOW ARE U ? ..." << std::endl;
	client->prepareMsg("HOW ARE U ?", 12);
	client->send();
	Sleep(1);
	std::cout << "CLIENT : SENDING LOL ..." << std::endl;
	client->prepareMsg("LOL", 4);
	client->send();
	Sleep(1);
	std::cout << "Creating new client ...." << std::endl;
	client2 = new WindowsTCPClient();
	Sleep(1);
	std::cout << "Connecting to server ... " << std::endl;
	client2->connectTo(ip, port);
	Sleep(1);
	std::cout << "CLIENT 2 : SENDING CONNECTION OK ..." << std::endl;
	client2->prepareMsg("CONNECTION OK", 14);
	client2->send();
	Sleep(1);
	std::cout << "CLIENT : SENDING STOP ... " << std::endl;
	client->prepareMsg("STOP", 5);
	client->send();
	client->closeSock();
	std::cout << "CLIENT2 : SENDING STOP ... " << std::endl;
	client2->prepareMsg("STOP", 5);
	client2->send();
	client2->closeSock();

	system("PAUSE");
}

int main()
{
	sampleServerTest();
}
