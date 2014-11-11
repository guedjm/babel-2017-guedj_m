#pragma once

# include <string>

class IWritable
{
public:
	static const int 	BUFF_SIZE = 1500;

	virtual ~IWritable() {}

	virtual int			getWriteFd() const = 0;

	virtual bool 		somethingToWrite() const = 0;
	virtual void		prepareMsg(std::string const &, int) = 0;
	virtual int			send() = 0;
};
