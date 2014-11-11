#include <algorithm>
#include <sstream>
#include "eErrorCode.h"
#include "constant.h"
#include "Serializer.hpp"
#include "Database.h"


Database::Database()
{
}


Database::~Database()
{
	this->clear();
}


// Core
void		Database::clear()
{
	this->_users.clear();
}

int			Database::addUser(std::string const& name, std::string const& password)
{
	if (this->getUser(name) != NULL)
		return eErrorCode::UnavailableUsername;
	this->_users.push_front(UserData(name, password));
	return eErrorCode::Success;
}

bool		Database::removeUser(UserData& user)
{
	std::list<UserData>::iterator target = this->_users.end();
	for (std::list<UserData>::iterator it = this->_users.begin(); it != this->_users.end(); ++it)
	{
		if (*it == user)
			target == it;
		else
		{
			it->getListFriend().remove(&user);
			it->getListFriendRequest().remove(&user);
		}
	}
	if (!(target != this->_users.end()))
		return false;
	this->_users.erase(target);
	return true;
}

bool		Database::removeUser(std::string const& name)
{
	UserData* user = this->getUser(name);
	bool success = user != 0;
	for (std::list<UserData>::iterator it = this->_users.begin(); it != this->_users.end(); ++it)
	{
		if (user == 0)
		{
			for (std::list<UserData*>::iterator kt = it->getListFriend().begin(); user == 0 && kt != it->getListFriend().end(); ++kt)
			{
				if ((*kt)->getName().compare(name) == 0)
					user = *kt;
			}
			for (std::list<UserData*>::iterator kt = it->getListFriendRequest().begin(); user == 0 && kt != it->getListFriendRequest().end(); ++kt)
			{
				if ((*kt)->getName().compare(name) == 0)
					user = *kt;
			}
		}
		if (user != 0)
		{
			it->getListFriend().remove(user);
			it->getListFriendRequest().remove(user);
		}
	}
	if (!success)
		return false;
	this->_users.remove (*user);
	return true;
}


int								Database::checkCredentials(UserData const& user, std::string const& password) const
{
	if (user.getPassword().compare(password) != 0)
		return eErrorCode::UnknownUser;
	return eErrorCode::Success;
}

int								Database::checkCredentials(std::string const& username, std::string const& password) const
{
	UserData const* user = this->getUser(username);
	if (user == 0 || user->getPassword().compare(password) != 0)
		return eErrorCode::UnknownUser;
	return eErrorCode::Success;
}


// Utils
bool							Database::hasUser(std::string const& name) const
{
	std::list<UserData>::const_iterator it;
	for (it = this->_users.begin(); it != this->_users.end() && it->getName().compare(name); ++it);
	return (it != this->_users.end());
}

UserData*						Database::getUser(std::string const& name)
{
	std::list<UserData>::iterator it;
	for (it = this->_users.begin(); it != this->_users.end() && it->getName().compare(name); ++it);
	return (it == this->_users.end() ? NULL : &(*it));

}

UserData const*					Database::getUser(std::string const& name) const
{
	std::list<UserData>::const_iterator it;
	for (it = this->_users.begin(); it != this->_users.end() && it->getName().compare(name); ++it);
	return (it == this->_users.end() ? NULL : &(*it));

}

std::list<UserData>::iterator	Database::getIterator(std::string const& name)
{
	std::list<UserData>::iterator it;
	for (it = this->_users.begin(); it != this->_users.end() && it->getName().compare(name); ++it);
	return it;
}


// Friends
int								Database::removeFriend(UserData& asker, std::string const& target)
{
	UserData* t = this->getUser(target);
	if (t == 0 || asker.isFriend(*t) == false)
		return eErrorCode::UnknownUser;
	t->removeFriend(asker);
	asker.removeFriend(*t);
	return eErrorCode::Success;
}

int								Database::addFriendRequest(UserData& asker, std::string const& target)
{
	UserData* t = this->getUser(target);
	if (t == 0)
		return eErrorCode::UnknownUser;
	if (asker.isFriend(*t))
		return eErrorCode::AlreadyFriend;
	if (asker.hasRequest(*t) || t->hasRequest(asker))
		return eErrorCode::RequestPending;
	if (asker.getListFriend().size() + asker.getListFriendRequest().size() >= MAX_FRIENDS)
		return eErrorCode::FriendsMaxedOut;
	t->addFriendRequest(asker);
	return eErrorCode::Success;
}

int								Database::validateFriendRequest(UserData& asker, std::string const& target)
{
	UserData* t = this->getUser(target);
	if (t == 0)
		return eErrorCode::UnknownUser;
	if (asker.isFriend(*t))
		return eErrorCode::AlreadyFriend;
	if (asker.hasRequest(*t) == false)
		return eErrorCode::UnknownUser;
	if (asker.getListFriend().size() + asker.getListFriendRequest().size() > MAX_FRIENDS)
		return eErrorCode::FriendsMaxedOut;
	asker.removeFriendRequest(*t);
	asker.addFriend(*t);
	t->addFriend(asker);
	return eErrorCode::Success;
}

int								Database::refuseFriendRequest(UserData& asker, std::string const& target)
{
	UserData* t = this->getUser(target);
	if (t == 0)
		return eErrorCode::UnknownUser;
	if (asker.isFriend(*t))
		return eErrorCode::AlreadyFriend;
	if (asker.hasRequest(*t) == false)
		return eErrorCode::UnknownUser;
	asker.removeFriendRequest(*t);
	return eErrorCode::Success;
}


// Serialization
std::string						Database::serialize() const
{
	std::string r = Serializer::serialize<int>(this->_users.size());
	for (std::list<UserData>::const_iterator it = this->_users.begin(); it != this->_users.end(); ++it)
		r += it->serialize();
	return r;
}

void							Database::deserialize(std::string const& s)
{
	std::istringstream iss(s);
	int size = Serializer::deserialize<int>(iss);

	std::list<std::string> *friends = new std::list<std::string>[size];
	std::list<std::string> *requests = new std::list<std::string>[size];

	for (int i = size - 1; i >= 0; --i)
	{
		std::string name = Serializer::deserialize<std::string>(iss);
		std::string password = Serializer::deserialize<std::string>(iss);
		this->_users.push_front(UserData(name, password));
		
		friends[i] = Serializer::deserialize<std::list<std::string>>(iss);
		requests[i] = Serializer::deserialize<std::list<std::string>>(iss);
	}

	std::list<UserData>::iterator usersIterator = this->_users.begin();
	for (int i = 0; i < size; ++i)
	{
		for (std::list<std::string>::iterator it = friends[i].begin(); it != friends[i].end(); ++it)
		{
			UserData* target = this->getUser(*it);
			if (target != 0)
				usersIterator->addFriend(*target);
		}
		for (std::list<std::string>::iterator it = requests[i].begin(); it != requests[i].end(); ++it)
		{
			UserData* target = this->getUser(*it);
			if (target != 0)
				usersIterator->addFriendRequest(*target);
		}
		++usersIterator;
	}
	delete[] friends;
	delete[] requests;
}


// Getters
std::list<UserData> const&	Database::getUsers() const
{
	return this->_users;
}

// Setters
void						Database::setUsers(std::list<UserData>& users)
{
	this->_users = users;
}
