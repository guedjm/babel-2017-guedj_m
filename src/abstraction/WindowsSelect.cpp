#ifdef _WIN32

#include <iostream>
#include <stdexcept>
#include <exception>
#include <algorithm>
#include <Windows.h>
#include <strsafe.h>
#include "constant.h"
#include "WindowsSelect.h"


WindowsSelect::WindowsSelect() : _stdin(0), _useTimeout(false), _maxFd(0), _sec(0), _usec(0)
{
}


WindowsSelect::~WindowsSelect()
{
}


void			WindowsSelect::addReadFd(IObservable *fd)
{
	if (fd->getReadFd() == 0)
		this->_stdin = fd;
	else if (std::find(this->_readFds.begin(), this->_readFds.end(), fd) == this->_readFds.end())
		this->_readFds.push_back(fd);
}

void			WindowsSelect::addWriteFd(IWritable *fd)
{
	if (std::find(this->_writeFds.begin(), this->_writeFds.end(), fd) == this->_writeFds.end())
		this->_writeFds.push_back(fd);
}


void			WindowsSelect::removeReadFd(IObservable *fd)
{
	if (fd->getReadFd() == 0)
		this->_stdin = 0;
	else
	{
		std::list<IObservable*>::iterator it = std::find(this->_readFds.begin(), this->_readFds.end(), fd);

		if (it != this->_readFds.end())
			this->_readFds.erase(it);
	}
}

void			WindowsSelect::removeWriteFd(IWritable *fd)
{
	std::list<IWritable*>::iterator it = std::find(this->_writeFds.begin(), this->_writeFds.end(), fd);

	if (it != this->_writeFds.end())
		this->_writeFds.erase(it);
}


void			WindowsSelect::setTimeout(long sec, long usec)
{
	if (sec == -1 || usec == -1)
		this->_useTimeout = false;
	else
	{
		this->_useTimeout = true;
		this->_sec = sec;
		this->_usec = usec;
	}
}

void			WindowsSelect::getRemainingTime(long &sec, long &usec) const
{
	sec = this->_timeout.tv_sec;
	usec = this->_timeout.tv_usec;
}


std::list<IObservable*> const	&WindowsSelect::getReadyRead() const
{
	return (this->_readyRead);
}

std::list<IWritable*> const	&WindowsSelect::getReadyWrite() const
{
	return (this->_readyWrite);
}


void				WindowsSelect::getTime()
{
	FILETIME		filetime;
	LARGE_INTEGER	tmp;

	GetSystemTimeAsFileTime(&filetime);
	tmp.HighPart = filetime.dwHighDateTime;
	tmp.LowPart = filetime.dwLowDateTime;

	this->_time = tmp.QuadPart;
}

void				WindowsSelect::updateTimeout(bool timeout)
{
	if (this->_useTimeout == false || timeout)
	{
		this->_timeout.tv_sec = 0;
		this->_timeout.tv_usec = 0;
	}
	else
	{
		FILETIME		filetime;
		LARGE_INTEGER	tmp;

		GetSystemTimeAsFileTime(&filetime);
		tmp.HighPart = filetime.dwHighDateTime;
		tmp.LowPart = filetime.dwLowDateTime;

		unsigned long long int diff = (tmp.QuadPart - this->_time) / 10;
		this->_timeout.tv_sec = (long)(diff / USEC_TO_SEC);
		this->_timeout.tv_usec = diff % USEC_TO_SEC;
	}
}


void ErrorExit(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
									  (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
					LocalSize(lpDisplayBuf) / sizeof(TCHAR),
					TEXT("%s failed with error %d: %s"),
					lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}


int				WindowsSelect::start()
{
	int			ret;
	fd_set		readFds;
	fd_set 		writeFds;

	bool movingOn = false;
	long targetSec = this->_sec;
	if (this->_useTimeout)
		this->getTime();
	while (!movingOn)
	{
		FD_ZERO(&readFds);
		FD_ZERO(&writeFds);
		this->_readyRead.clear();
		this->_readyWrite.clear();
		this->setFdSet(&readFds, &writeFds);

		if (this->_useTimeout && targetSec < 1)
		{
			this->_timeout.tv_sec = 0;
			this->_timeout.tv_usec = this->_usec;
			movingOn = true;
		}
		else
		{
			this->_timeout.tv_sec = 1;
			this->_timeout.tv_usec = 0;
			if (this->_useTimeout)
				targetSec -= 1;
		}
		ret = select(0, &readFds, &writeFds, 0, (this->_useTimeout || this->_stdin != 0) ? &this->_timeout : 0);
		if (ret < 0)
		{
			ErrorExit("Select");
			throw std::runtime_error("Select failed");
		}
		if (this->_stdin != 0 && this->keyWasPressed())
		{
			this->_readyRead.push_back(this->_stdin);
			movingOn = true;
		}
		if (ret > 0)
			movingOn = true;
	}
	this->updateTimeout(ret == 0);
	if (ret > 0)
		this->getReadyFd(&readFds, &writeFds);
	return (ret);
}


bool			WindowsSelect::keyWasPressed() const
{
	HANDLE stdinHandle = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD buffer[128];
	DWORD cNumRead;
	bool pressed = false;
	if (!PeekConsoleInput(stdinHandle, buffer, 128, &cNumRead))
		throw std::runtime_error("PeekConsoleInput failed");
	for (int i = 0; !pressed && i < cNumRead; i++)
	{
		if (buffer[i].EventType == KEY_EVENT)
			pressed = true;
	}
	return pressed;
}

void			WindowsSelect::setFdSet(fd_set *readFds, fd_set *writeFds)
{
	int			fd;

	for (std::list<IObservable*>::iterator it = this->_readFds.begin();
		 it != this->_readFds.end(); ++it)
	{
		fd = ((*it)->getReadFd());
		FD_SET(fd, readFds);
	}
	for (std::list<IWritable*>::iterator it = this->_writeFds.begin();
		 it != this->_writeFds.end(); ++it)
	{
		fd = ((*it)->getWriteFd());
		FD_SET(fd, writeFds);
	}
}

void			WindowsSelect::getReadyFd(fd_set* readFds, fd_set* writeFds)
{
	for (std::list<IObservable*>::iterator it = this->_readFds.begin();
		 it != this->_readFds.end(); ++it)
	{
		if (FD_ISSET((*it)->getReadFd(), readFds))
			this->_readyRead.push_back(*it);
	}
	for (std::list<IWritable*>::iterator it = this->_writeFds.begin();
		 it != this->_writeFds.end(); ++it)
	{
		if (FD_ISSET((*it)->getWriteFd(), writeFds))
			this->_readyWrite.push_back(*it);
	}
}

#endif