#include "WindowsTCPServer.h"
#include "WindowsTCPClient.h"
#include "ITCPRemoteClient.h"
#include "ITCPClient.h"
#include <iostream>

void	testAccept()
{
	WindowsTCPServer TCPServer;
	WindowsTCPClient TCPClient;
	ITCPRemoteClient *remoteClient;
	int					ret = 0;
	std::string				msg = "bonjour";

	std::cout << "STARTING SERVER ..." << std::endl;
	TCPServer.start("", 4242, 10);
	std::cout << "CONNECTING TO SERVER ... " << std::endl;
	TCPClient.connectTo("127.0.0.1", 4242);
	std::cout << "Accepting Client ..." << std::endl;
	remoteClient = TCPServer.acceptClient();
	std::cout << "Preparing message and trying to send" << std::endl;
	remoteClient->prepareMsg("J'ai faim", 10);
	if (remoteClient->somethingToWrite() == true)
		ret = remoteClient->send();
	std::cout << ret << "bytes sended" << std::endl;
	if (remoteClient->somethingToWrite())
		std::cout << "Failed, still something to write" << std::endl;
	ret = TCPClient >> msg;
	std::cout << ret << "bytes received :" << msg << std::endl;
	delete remoteClient;
	ret = TCPClient >> msg;
	std::cout << ret << " is connected" << std::endl;
	system("PAUSE");
}