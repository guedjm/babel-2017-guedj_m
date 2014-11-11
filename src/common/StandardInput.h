#pragma once

# include "IObservable.h"

class StandardInput : public IObservable
{
public:
	StandardInput();
	~StandardInput();

	virtual int			getReadFd() const;

private:
	int	const			_fd;
};
