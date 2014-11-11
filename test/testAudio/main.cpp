#include "IOControl.h"
#include "opus.h"

int			main()
{
	int				size;
	IOControl		ctrl;
	unsigned char			inputBuff[10000];

	ctrl.startFull();

	while (1)
	{
		size = ctrl.copyInput(inputBuff, 10000);
		if (size != 0)
			size = ctrl.enqueueOutput(inputBuff, size);
		Pa_Sleep(10);
	}
}