#include <algorithm>
#include <sstream>

#include "UserData.h"
#include "Serializer.hpp"



UserData::UserData()
{
	this->_name = "";
	this->_password = "";
}

UserData::UserData(std::string const& name, std::string const& password)
{
	this->_name = name;
	this->_password = password;
}
UserData::UserData(std::string const& name, std::string const& password, 
				   std::list<UserData *> friendList, std::list<UserData *> friendListRequest)
{
	this->_name = name;
	this->_password = password;
	this->_friendList = friendList;
	this->_friendListRequest = friendListRequest;
}

UserData::~UserData()
{

}


// Serialization

std::string				UserData::serialize() const
{
	std::string r = Serializer::serialize<std::string>(this->_name);
	r += Serializer::serialize<std::string>(this->_password);
	std::list<std::string> tmp;
	for (std::list<UserData*>::const_iterator it = this->_friendList.begin(); it != this->_friendList.end(); ++it)
		tmp.push_back((*it)->getName());
	r += Serializer::serialize<std::list<std::string>>(tmp);
	tmp.clear();
	for (std::list<UserData*>::const_iterator it = this->_friendListRequest.begin(); it != this->_friendListRequest.end(); ++it)
		tmp.push_back((*it)->getName());
	r += Serializer::serialize<std::list<std::string>>(tmp);
	return r;
}


// --- BEGIN --- GETTERS AND SETTERS // 

std::string	const&	UserData::getName() const
{
	return (this->_name);
}

void				UserData::setName(std::string const& name)
{
	this->_name = name;
}

std::string	const&	UserData::getPassword() const
{
	return (this->_password);
}

void				UserData::setPassword(std::string const& password)
{
	this->_password = password;
}

std::list<UserData*>	&UserData::getListFriend()
{
	return (this->_friendList);
}

void					UserData::setListFriend(std::list<UserData *> &listFriend)
{
	this->_friendList = listFriend;
}

std::list<UserData*>	&UserData::getListFriendRequest()
{
	return (this->_friendListRequest);
}

void					UserData::setListFriendRequest(std::list<UserData *> &friendListRequest)
{
	this->_friendListRequest = friendListRequest;
}

// --- END --- GETTERS AND SETTERS //


// --- BEGIN --- CORE FUNCTIONS //

bool					UserData::addFriend(UserData& user)
{
	std::list<UserData *>::iterator it;
	for (it = this->_friendList.begin(); it != this->_friendList.end() && **it != user; ++it);
	if (!(it != this->_friendList.end()))
	{
		this->_friendList.push_back(&user);
		return true;
	}
	return false;
}

bool					UserData::removeFriend(UserData& user)
{
	std::list<UserData *>::iterator it;
	for (it = this->_friendList.begin(); it != this->_friendList.end() && **it != user; ++it);
	if (it != this->_friendList.end())
	{
		this->_friendList.erase(it);
		return true;
	}
	return false;
}

bool					UserData::addFriendRequest(UserData& user)
{
	std::list<UserData *>::iterator it;
	for (it = this->_friendListRequest.begin(); it != this->_friendListRequest.end() && **it != user; ++it);
	if (!(it != this->_friendListRequest.end()))
	{
		this->_friendListRequest.push_back(&user);
		return true;
	}
	return false;
}

bool					UserData::removeFriendRequest(UserData& user)
{
	std::list<UserData *>::iterator it;
	for (it = this->_friendListRequest.begin(); it != this->_friendListRequest.end() && **it != user; ++it);
	if (it != this->_friendListRequest.end())
	{
		this->_friendListRequest.erase(it);
		return true;
	}
	return false;
}

bool						UserData::isFriend(UserData const& user) const
{
	std::list<UserData *>::const_iterator it;
	for (it = this->_friendList.begin(); it != this->_friendList.end() && **it != user; ++it);
	return it != this->_friendList.end();
}

bool						UserData::isFriend(std::string const& user) const
{
	std::list<UserData *>::const_iterator it;
	for (it = this->_friendList.begin(); it != this->_friendList.end() && (*it)->getName().compare(user) != 0; ++it);
	return it != this->_friendList.end();
}

bool						UserData::hasRequest(UserData const& user) const
{
	std::list<UserData *>::const_iterator it;
	for (it = this->_friendListRequest.begin(); it != this->_friendListRequest.end() && **it != user; ++it);
	return it != this->_friendListRequest.end();
}

bool						UserData::hasRequest(std::string const& user) const
{
	std::list<UserData *>::const_iterator it;
	for (it = this->_friendListRequest.begin(); it != this->_friendListRequest.end() && (*it)->getName().compare(user) != 0; ++it);
	return it != this->_friendListRequest.end();
}

// --- END --- CORE FUNCTIONS //

