#ifdef _CLIENT

#include "QTMutex.h"

QTMutex::QTMutex()
{
}

QTMutex::~QTMutex()
{
}

void		QTMutex::lock()
{
	this->_mut.lock();
}

bool		QTMutex::tryLock()
{
	return (this->_mut.tryLock());
}

void		QTMutex::unLock()
{
	this->_mut.unlock();
}

#endif