#include <iostream>
#include "BabelServer.h"
#include "FileSerializer.h"
#include "StandardInput.h"
#include "constant.h"


BabelServer::BabelServer(int port) : _port(port), _running(true)
{
	FileSerializer fileSerializer(DEFAULT_SERVER_DB_FILE);
	std::string data = fileSerializer.readFromFile();
	if (data.size() > 0)
		this->_database.deserialize(data);
}


BabelServer::~BabelServer()
{
	FileSerializer fileSerializer(DEFAULT_SERVER_DB_FILE);
	fileSerializer.writeToFile(this->_database.serialize());
}


void					BabelServer::start()
{
	std::cout << "> Server started." << std::endl;
	this->_server.start();
	this->loop();
	this->_server.stop();
}

void					BabelServer::setSelectTimeout(mySelect& bs)
{
	unsigned long long int timeout = (unsigned long long int)-1;
	for (std::list<std::pair<ITCPRemoteClient*, unsigned long long int>>::iterator it = this->_strangers.begin(); it != this->_strangers.end(); ++it)
	{
		if (it->second < timeout)
			timeout = it->second;
	}
	if (timeout == (unsigned long long int)-1)
	{
		this->_lastTimeout = 0;
		bs.setTimeout(-1, -1);
	}
	else
	{
		this->_lastTimeout = timeout;
		bs.setTimeout(timeout / USEC_TO_SEC, timeout % USEC_TO_SEC);
	}
}

void					BabelServer::updateStrangersTimeout(mySelect& bs)
{
	if (this->_lastTimeout != 0)
	{
		long sec;
		long usec;
		bs.getRemainingTime(sec, usec);
		unsigned long long int timeDiff = this->_lastTimeout - sec * USEC_TO_SEC - usec;
		for (std::list<std::pair<ITCPRemoteClient*, unsigned long long int>>::iterator it = this->_strangers.begin(); it != this->_strangers.end(); ++it)
		{
			if (it->second <= timeDiff)
				it->second = 0;
			else
				it->second -= timeDiff;
		}
	}
}

void					BabelServer::process(mySelect& bs)
{
	std::list<IWritable*> const& writeReady = bs.getReadyWrite();
	for (std::list<IWritable*>::const_iterator it = writeReady.begin(); it != writeReady.end(); ++it)
	{
		(*it)->send();
	}

	std::list<IObservable*> const& readReady = bs.getReadyRead();
	for (std::list<IObservable*>::const_iterator it = readReady.begin(); it != readReady.end(); ++it)
	{
		if ((*it)->getReadFd() == 0)
		{
			this->_running = false;
			std::cout << "> Input detected. Stopping the server." << std::endl;
		}
		else if (*it == &this->_server)
		{
			ITCPRemoteClient* newClient = this->_server.acceptClient();
			this->_strangers.push_front(std::pair<ITCPRemoteClient*, unsigned long long int>(newClient, AUTH_TIMEOUT));
			std::cout << "Guest connection: " + newClient->getIp() << std::endl;
		}
	}
}

void					BabelServer::updateData(mySelect& bs)
{
	std::list<std::pair<ITCPRemoteClient*, unsigned long long int>> toRemove;
	for (std::list<std::pair<ITCPRemoteClient*, unsigned long long int>>::iterator it = this->_strangers.begin(); it != this->_strangers.end(); ++it)
	{
		if (it->first->mustBeClosed() || it->second == 0)
		{
			toRemove.push_front(*it);
		}
	}
	for (std::list<std::pair<ITCPRemoteClient*, unsigned long long int>>::iterator it = toRemove.begin(); it != toRemove.end(); ++it)
	{
		std::cout << "Guest authentification timeout: " + it->first->getIp() << std::endl;
		bs.removeReadFd(it->first);
		delete it->first;
		this->_strangers.remove(*it);
	}
}


void					BabelServer::loop()
{
	mySelect babelSelect;
	StandardInput standardInput;
	babelSelect.addReadFd(&this->_server);
	babelSelect.addReadFd(&standardInput);
	while (this->_running)
	{
		this->setSelectTimeout(babelSelect);
		babelSelect.start();
		this->updateStrangersTimeout(babelSelect);
		this->process(babelSelect);
		this->updateData(babelSelect);
	}
}
