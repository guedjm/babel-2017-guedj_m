#pragma once

# include <list>
# include <utility>
# include "IObservable.h"
# include "IWritable.h"

class ISelect
{

public:
	virtual ~ISelect() {}
	
	virtual void					addReadFd(IObservable*) = 0;
	virtual void 					addWriteFd(IWritable*) = 0;

	virtual void					removeReadFd(IObservable*) = 0;
	virtual void					removeWriteFd(IWritable*) = 0;

	virtual void					setTimeout(long, long) = 0;
	virtual void					getRemainingTime(long &, long &) const = 0;

	virtual int 					start() = 0;
	virtual std::list<IObservable*> const	&getReadyRead() const = 0;
	virtual std::list<IWritable*> const		&getReadyWrite() const = 0;
};
