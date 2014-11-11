#ifdef _WIN32

#pragma once

#include <Windows.h>
#include "IMutex.h"

class WindowsMutex : public IMutex
{
 protected:
  CRITICAL_SECTION	_mutex;

 public:
  WindowsMutex();
  virtual ~WindowsMutex();

  void			lock();
  void			unLock();
  bool			tryLock();
  CRITICAL_SECTION	*getMutex();
};

#endif
