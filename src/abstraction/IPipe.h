#pragma once

# include "IReadable.h"
# include "IWritable.h"

class IPipe : public IReadable, public IWritable
{
public:
	virtual ~IPipe() {}

	virtual int			getReadFd() const = 0;
	virtual int			getWriteFd() const = 0;

	virtual bool 		somethingToWrite() const = 0;
	virtual void		prepareMsg(std::string const &, int) = 0;
	virtual int			operator>>(std::string &) = 0;

	virtual int 		send() = 0;
};
