#ifdef _WIN32

#pragma once

# include <list>
# include <winsock2.h>
# include <ws2tcpip.h>
# include <windows.h>

# include "ISelect.h"

class WindowsSelect : public ISelect
{
public:
	WindowsSelect();
	virtual ~WindowsSelect();

	virtual void	addReadFd(IObservable*);
	virtual void	addWriteFd(IWritable*);

	virtual void	removeReadFd(IObservable*);
	virtual void	removeWriteFd(IWritable*);

	virtual	void	setTimeout(long, long);
	virtual	void 	getRemainingTime(long&, long&) const;

	virtual int		start();

	virtual std::list<IObservable*> const	&getReadyRead() const;
	virtual std::list<IWritable*> const 	&getReadyWrite() const;


private:
	void					setFdSet(fd_set*, fd_set*);
	void		 			getReadyFd(fd_set*, fd_set*);

	void					getTime();
	void					updateTimeout(bool);

	bool					keyWasPressed() const;

	IObservable*			_stdin;
	bool					_useTimeout;
	unsigned long long int	_time;
	int						_maxFd;
	long					_sec;
	long					_usec;
	struct timeval			_timeout;
	std::list<IObservable*>	_readFds;
	std::list<IWritable*>	_writeFds;
	std::list<IObservable*>	_readyRead;
	std::list<IWritable*>	_readyWrite;

};

#endif