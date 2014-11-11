#include "ClientData.h"
#include "constant.h"
#include "myMutex.h"

ClientData::ClientData():
    _serverPort(DEFAULT_SERVER_PORT)
{
	this->_userMutex = new myMutex();
	this->_serverMutex = new myMutex();
	if (this->_userMutex == 0 || this->_serverMutex == 0)
		throw std::exception();
}

ClientData::~ClientData()
{
	delete this->_userMutex;
	delete this->_serverMutex;
}

void		ClientData::lockUser()
{
	this->_userMutex->lock();
}

User		&ClientData::getUser()
{
	return (this->_user);
}

void		ClientData::unlockUser()
{
	this->_userMutex->unLock();
}

void		ClientData::lockServer()
{
	this->_serverMutex->lock();
}

QString const	&ClientData::getServerIp() const
{
	return (this->_serverIp);
}

void            ClientData::setServerIp(const QString &ip)
{
    this->_serverIp = ip;
}

void			ClientData::setAuthType(eAuthType authType)
{
	this->_authType = authType;
}

void		ClientData::unlockServer()
{
	this->_serverMutex->unLock();
}

unsigned short	ClientData::getServerPort() const
{
	return (this->_serverPort);
}

eAuthType		ClientData::getAuthType() const
{
	return (this->_authType);
}
