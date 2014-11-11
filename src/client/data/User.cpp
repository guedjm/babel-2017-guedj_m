#include "User.h"

User::User()
{
	this->_name = "";
    this->_password = "";
}

User::~User()
{
}

void		User::setConnected(bool connected)
{
	this->_connected = connected;
}

void		User::setIdent(QString const &name, QString const &pwd)
{
	this->_name = name;
    this->_password = pwd;
}

void		User::addFriend(QString const &name, eFriendStatus status)
{
	this->_friends.push_back(Friend(name, status));
}

void		User::removeFriend(QString const &name)
{
	std::list<Friend>::iterator it = this->_friends.begin();
	for (; it != this->_friends.end(); ++it)
	{
		if (it->getName() == name)
		{
			this->_friends.erase(it);
			break;
		}
	}
}

void		User::friendSetStatus(QString const &name, eFriendStatus status)
{
    bool    exist = false;

	std::list<Friend>::iterator it = this->_friends.begin();
	for (; it != this->_friends.end(); ++it)
	{
		if (it->getName() == name)
		{
            exist = true;
			it->setStatus(status);
			break;
		}
	}
    if (!exist)
        this->addFriend(name, status);
}

bool				User::isConnected() const
{
	return (this->_connected);
}

QString const	&User::getName() const
{
	return (this->_name);
}

QString const   &User::getPassword() const
{
    return (this->_password);
}

std::list<Friend> const	&User::getFriendList() const
{
	return (this->_friends);
}

