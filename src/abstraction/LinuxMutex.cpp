#ifndef _WIN32

#include "LinuxMutex.h"

LinuxMutex::LinuxMutex()
{
  pthread_mutex_init(&this->_mutex, NULL);
}

LinuxMutex::~LinuxMutex()
{
  pthread_mutex_destroy(&this->_mutex);
}

void			LinuxMutex::lock()
{
  pthread_mutex_lock(&this->_mutex);
}

void			LinuxMutex::unLock()
{
  pthread_mutex_unlock(&this->_mutex);
}

bool			LinuxMutex::tryLock()
{
  if (pthread_mutex_trylock(&this->_mutex) == EBUSY)
    return false;
  return true;
}

pthread_mutex_t		*LinuxMutex::getMutex()
{
  return (&this->_mutex);
}

#endif
