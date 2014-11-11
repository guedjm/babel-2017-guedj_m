#pragma once

#include "User.h"
#include "IMutex.h"
#include "eAuthType.h"

class ClientData
{
public:
	ClientData();
	~ClientData();

	void					lockUser();
	User					&getUser();
	void					unlockUser();

	void					lockServer();
    QString	const           &getServerIp() const;
	void					unlockServer();
	unsigned short			getServerPort() const;
	eAuthType				getAuthType() const;

    void					setServerIp(QString const &);
	void					setAuthType(eAuthType);
private:
	IMutex					*_userMutex;
	User					_user;
	
	IMutex					*_serverMutex;
    QString             	_serverIp;
	const unsigned short	_serverPort;
	eAuthType				_authType;
};
