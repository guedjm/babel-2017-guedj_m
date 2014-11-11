# include "StandardInput.h"

StandardInput::StandardInput() : _fd(0)
{
}

StandardInput::~StandardInput()
{
}

int			StandardInput::getReadFd() const
{
	return this->_fd;
}
