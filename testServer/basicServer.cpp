#include <iostream>
#include <sstream>
#include "ITCPServer.h"
#include "myTCPServer.h"
#include "PacketHelper.h"
#include "TCPPacketHeader.h"
#include "Serializer.hpp"
#include "constant.h"

int				runBasicServer()
{
	char					magic1;
	char					magic2;
	char					magic3;
	int						packetId;
	int						packetLen;
	std::string				ip;
	unsigned short			port;
	std::string				packet;
	int						size;
	struct TCPPacketHeader	header;

	ITCPServer	*server = new myTCPServer();

	port = DEFAULT_SERVER_PORT;
	server->start(ip, port, 10); //On démare le server

	std::cout << "Server started" << std::endl;

	ITCPRemoteClient *client = server->acceptClient(); // On accepte la connexion du client
	std::cout << "Client connected" << std::endl;
	
	
	PacketHelper::writeTcpHeader(packet, RequestAuth);
	PacketHelper::writeTcpHeaderSize(packet);	//Creer un packet vide (avec command id = RequestAuth)
	client->prepareMsg(packet, packet.length());
	client->send(); //On send
	std::cout << "Auth msg sended" << std::endl;

	{
		packet = "";
		size = (*client >> packet);  //On reçois
		std::istringstream stream(packet);
		PacketHelper::readTcpHeader(stream, header); // On read le header reçu
		std::cout << "Receive packet with id " << header.commandId << std::endl;
		std::cout << "Payload size " << header.payloadSize << std::endl;
		if (header.commandId != SignIn)
			throw std::runtime_error("Invalid packet");
		std::string usr = Serializer::deserialize<std::string>(stream); // username
		std::string passwd = Serializer::deserialize<std::string>(stream); //password
		std::cout << "Identification username = " << usr << " password : " << passwd << std::endl;
		system("PAUSE");

		//En answer
	}
}