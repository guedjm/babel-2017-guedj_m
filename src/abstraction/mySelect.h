#pragma once

#include "LinuxSelect.h"
#include "WindowsSelect.h"

class mySelect :
#ifdef _WIN32
	public WindowsSelect
#else
	public LinuxSelect
#endif
{
public:
	mySelect() {};
	~mySelect() {};
};