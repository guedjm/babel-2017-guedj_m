#pragma once

#include <list>
#include <utility>
#include "constant.h"
#include "Database.h"
#include "UserData.h"
#include "mySelect.h"
#include "myTCPServer.h"
#include "myTCPRemoteClient.h"

class BabelServer
{
public:
	BabelServer(int = DEFAULT_SERVER_PORT);
	~BabelServer();

public:
	void							start();


private:
	void							loop();
	void							setSelectTimeout(mySelect&);
	void							updateStrangersTimeout(mySelect&);
	void							process(mySelect&);
	void							updateData();

private:
	Database						_database;
	myTCPServer						_server;

	std::list<std::pair<ITCPRemoteClient*,
		unsigned long long int>>	_strangers;
	std::list<std::pair<ITCPRemoteClient*,
		UserData&>>					_clients;

	short							_port;
	unsigned long long int			_lastTimeout;
	bool							_running;

};

