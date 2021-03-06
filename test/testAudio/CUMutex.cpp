#ifndef _WIN32

#include "CUMutex.h"

CUMutex::CUMutex()
{
  pthread_mutex_init(&this->_mutex, NULL);
}

CUMutex::~CUMutex()
{
  pthread_mutex_destroy(&this->_mutex);
}

void			CUMutex::lock()
{
  pthread_mutex_lock(&this->_mutex);
}

void			CUMutex::unLock()
{
  pthread_mutex_unlock(&this->_mutex);
}

bool			CUMutex::tryLock()
{
  if (pthread_mutex_trylock(&this->_mutex) == EBUSY)
    return false;
  return true;
}

pthread_mutex_t		*CUMutex::getMutex()
{
  return (&this->_mutex);
}

#endif
