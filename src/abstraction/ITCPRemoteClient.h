#pragma once

# include "IWritable.h"
# include "IReadable.h"

class ITCPRemoteClient : public IWritable, public IReadable
{
public:
	virtual ~ITCPRemoteClient() {}

	virtual int 				getWriteFd() const = 0;
	virtual int 				getReadFd() const = 0;
	virtual std::string const	&getIp() const = 0;

	virtual void				prepareClose() = 0;
	virtual bool 				mustBeClosed() const = 0;

	virtual int 		operator>>(std::string &) = 0;
	virtual void 		prepareMsg(std::string const &, int) = 0;
	virtual int 		send() = 0;
};
