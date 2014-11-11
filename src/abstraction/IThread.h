#pragma once
class IThread
{
public:
	IThread();
	~IThread();

	virtual void create(void (*fpt)(void *)) = 0;
	virtual int	wait() = 0;
	virtual int	destroy() = 0;
	virtual int run() = 0;
};

