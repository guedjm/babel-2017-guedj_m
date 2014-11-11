#pragma once

# include "IPipe.h"

class LinuxPipe : public IPipe
{
public:
	LinuxPipe();
	LinuxPipe(LinuxPipe const &);
	virtual ~LinuxPipe();

	virtual int			getReadFd() const;
	virtual int			getWriteFd() const;

	virtual bool 			somethingToWrite() const;
	virtual void 			prepareMsg(std::string const &, int);
	virtual int				operator>>(std::string &);

	virtual int			send();

private:
	int			_pipeFd[2];
	int 		_toSendLen;
	std::string	_toSend;
};
