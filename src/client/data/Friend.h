#pragma once

#include <QString>
#include "eFriendStatus.h"

class Friend
{
public:
    Friend(QString const &, eFriendStatus);
	~Friend();

    QString const       &getName() const;
	eFriendStatus		getStatus() const;

	void				setStatus(eFriendStatus);

private:
    QString         	_name;
	eFriendStatus		_status;
};
