#include <iostream>
#include <fstream>
#include <stdio.h>
#include "IOControl.h"

int			main()
{
	IOControl	ctrl;
	short		buff[RECORDING_BUFF_SIZE];

#ifdef RECORD
	ctrl.startRecord();
	while (!ctrl.streamEnded())
		Pa_Sleep(100);
	std::cout << "Recording done" << std::endl;

	ctrl.copyInput(buff);

	std::ofstream file(FILENAME, std::ios::out | std::ios::binary);
	file.write((char*)buff, RECORDING_BUFF_SIZE * sizeof(short));
	file.close();

#else
	ctrl.startPlay();
	while (!ctrl.streamEnded())
		Pa_Sleep(100);
	std::cout << "Recording done" << std::endl;

#endif

	return (0);
}