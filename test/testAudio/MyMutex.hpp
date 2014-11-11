#include "CWMutex.h"
#include "CUMutex.h"

class myMutex :
#ifdef _WIN32
public CWMutex
#else
public CUMutex
#endif
{
public:
  myMutex(){};
  ~myMutex(){};
};
