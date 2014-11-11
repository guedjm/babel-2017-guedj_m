#pragma once
#ifdef _CLIENT

#include <QMutex.h>
#include "IMutex.h"

class QTMutex : public IMutex
{
public:
	QTMutex();
	virtual ~QTMutex();

	virtual void	lock();
	virtual bool	tryLock();
	virtual void	unLock();

private:
	QMutex			_mut;
};

#endif