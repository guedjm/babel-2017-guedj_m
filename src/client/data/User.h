#pragma once

#include <list>
#include "Friend.h"

class User
{
public:
	User();
	~User();

	void				setConnected(bool);
    void				setIdent(QString const &, QString const &);
    void				addFriend(QString const &, eFriendStatus);
    void				removeFriend(QString const &);
    void				friendSetStatus(QString const &, eFriendStatus);

	bool					isConnected() const;
    QString const		&getName() const;
    QString const       &getPassword() const;
	std::list<Friend> const &getFriendList() const;

private:
	bool				_connected;
    QString			_name;
    QString         _password;
	std::list<Friend>	_friends;
};
