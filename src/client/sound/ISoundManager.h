#pragma once

class ISoundManager
{
public:
	~ISoundManager() {}

	virtual int		start() = 0;
};
