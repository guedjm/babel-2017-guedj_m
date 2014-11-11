#include "QTMutex.h"
#include "WindowsMutex.h"
#include "LinuxMutex.h"

class myMutex :
#ifdef _CLIENT
	public QTMutex
#else
	#ifdef _WIN32
		public WindowsMutex
	#else
		public LinuxMutex
	#endif
#endif
{
public:
	virtual ~myMutex() {};
};
