#pragma once

#include <string>
#include <list>

class UserData
{
public:
	UserData();
	UserData(std::string const&, std::string const&);
	UserData(std::string const&, std::string const&, std::list<UserData*>, std::list<UserData*>);
	~UserData();

	// Getters
	std::string const&			getName() const;
	std::string	const&			getPassword() const;

	std::list<UserData*>		&getListFriendRequest();
	std::list<UserData*>		&getListFriend();

	std::list<UserData*> const&	getListFriendRequest() const;
	std::list<UserData*> const&	getListFriend() const;

	// Setters
	void						setName(std::string const&);
	void						setPassword(std::string const&);
	void						setListFriendRequest(std::list<UserData*> &);
	void						setListFriend(std::list<UserData*> &);

	// Serialization
	std::string					serialize() const;


	// Core Functions
	bool						addFriend(UserData&);
	bool						addFriendRequest(UserData&);
	bool						removeFriend(UserData&);
	bool						removeFriendRequest(UserData&);

	bool						isFriend(UserData const&) const;
	bool						isFriend(std::string const&) const;

	bool						hasRequest(UserData const&) const;
	bool						hasRequest(std::string const&) const;


private:

	std::string						_name;
	std::string						_password;
	std::list<UserData*>			_friendListRequest;
	std::list<UserData*>			_friendList;
};


// Comparison Operators Overload
inline bool operator==(UserData const& lhs, UserData const& rhs)
{
	return lhs.getName().compare(rhs.getName()) == 0;
}

inline bool operator!=(UserData const& lhs, UserData const& rhs)
{
	return !operator==(lhs, rhs);
}
