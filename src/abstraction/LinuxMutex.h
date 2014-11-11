#ifndef _WIN32

#include <pthread.h>
#include <errno.h>
#include "IMutex.h"

class LinuxMutex : public IMutex
{
protected:
    pthread_mutex_t _mutex;

public:
    LinuxMutex();
    ~LinuxMutex();

    void lock();
    void unLock();
    bool tryLock();

    pthread_mutex_t *getMutex();
};

#endif
