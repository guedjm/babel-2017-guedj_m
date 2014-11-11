#include "Friend.h"

Friend::Friend(QString const &name, eFriendStatus status)
{
	this->_name = name;
	this->_status = status;
}

Friend::~Friend()
{
}

QString const		&Friend::getName() const
{
	return (this->_name);
}

eFriendStatus			Friend::getStatus() const
{
	return (this->_status);
}

void					Friend::setStatus(eFriendStatus status)
{
	this->_status = status;
}
