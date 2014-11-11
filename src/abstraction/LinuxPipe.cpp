#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <exception>
#include "LinuxPipe.h"

LinuxPipe::LinuxPipe()
{
	int		result;

	this->_toSendLen = 0;
	result = pipe(this->_pipeFd);
	if (result == -1)
		throw std::exception();
}

LinuxPipe::LinuxPipe(LinuxPipe const &pipe)
{
	this->_toSendLen = 0;
	this->_pipeFd[0] = pipe._pipeFd[0];
	this->_pipeFd[1] = pipe._pipeFd[1];
}

LinuxPipe::~LinuxPipe()
{
	close(this->_pipeFd[0]);
	close(this->_pipeFd[1]);
}

int			LinuxPipe::getReadFd() const
{
	return (this->_pipeFd[0]);
}

int			LinuxPipe::getWriteFd() const
{
	return (this->_pipeFd[1]);
}

bool 			LinuxPipe::somethingToWrite() const
{
	return (this->_toSendLen != 0);
}

void			LinuxPipe::prepareMsg(std::string const &, int)
{
	//TODO
}

int			LinuxPipe::operator>>(std::string &data)
{
	int		ret;
	int		totalSize;
	char	buff[IWritable::BUFF_SIZE];

	totalSize = 0;
	while (totalSize == 0 || ret == IWritable::BUFF_SIZE - 1)
	{
		ret = read(this->_pipeFd[0], buff, IWritable::BUFF_SIZE - 1);
		if (ret == -1)
			throw std::exception();
		data.append(buff, ret);
		totalSize += ret;
	}
	return (totalSize);
}

int			LinuxPipe::send()
{
	int		ret;

	ret = write(this->_pipeFd[1],
				this->_toSend.c_str(),
				this->_toSendLen);
	if (ret == -1)
		throw std::exception();
	if (ret != this->_toSendLen)
	{
		this->_toSend = this->_toSend.substr(ret);
		this->_toSendLen -= ret;
	}
	else
	{
		this->_toSend.clear();
		this->_toSendLen = 0;
	}
	return (ret);
}
