#include "CUThread.h"
#include "CWThread.h"

template<typename T, typename U, typename V>
class myThread :
#ifdef _WIN32
  public CWThread<T, U, V>
#else
  public CUThread<T, U, V>
#endif
{
public:
	myThread();
	~myThread();
};

