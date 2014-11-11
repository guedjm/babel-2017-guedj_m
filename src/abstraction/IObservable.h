#pragma once

class IObservable
{
public:
	virtual ~IObservable() {}

	virtual int 		getReadFd() const = 0;
};
