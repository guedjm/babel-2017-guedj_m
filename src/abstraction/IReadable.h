#pragma once

# include <string>
# include "IObservable.h"

class IReadable : public IObservable
{
public:
	virtual ~IReadable() {}

	virtual int			getReadFd() const = 0;
	virtual	int			operator>>(std::string &) = 0;
};
