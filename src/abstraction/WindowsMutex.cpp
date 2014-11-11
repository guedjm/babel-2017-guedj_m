#ifdef _WIN32

#include "WindowsMutex.h"

WindowsMutex::WindowsMutex()
{
  InitializeCriticalSection(&_mutex);
}

WindowsMutex::~WindowsMutex()
{
  DeleteCriticalSection(&_mutex);
}

void			WindowsMutex::lock()
{
  EnterCriticalSection(&_mutex);
}

void			WindowsMutex::unLock()
{
  LeaveCriticalSection(&_mutex);
}

bool			WindowsMutex::tryLock()
{
  return (TryEnterCriticalSection(&_mutex) != 0);
}

CRITICAL_SECTION	*WindowsMutex::getMutex()
{
  return (&this->_mutex);
}

#endif
