#pragma once

#include <fstream>
#include <iostream>
#include "portaudio.h"
#include "constant.h"

int			IOCallback(const void *, void *, unsigned long, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void *);

class IOControl
{
public:
	IOControl();
	~IOControl();

#ifdef RECORD
	int		startRecord();
	int		recordCallback(const void *, unsigned long);
	int		copyInput(short *);

#else

	int		startPlay();
	int		playCallback(void *, unsigned long);
	int		copyOutput(short *);
#endif

	bool	streamEnded() const;

private:
	PaStream			*_stream;

#ifdef RECORD
	PaStreamParameters	_inputParam;
	short				_inputBuff[RECORDING_BUFF_SIZE];
	unsigned int		_inputIndex;
#else
	PaStreamParameters	_outputParam;
	short				_outputBuff[RECORDING_BUFF_SIZE];
	unsigned int		_outputIndex;

	std::ifstream		_f1;
	std::ifstream		_f2;

#endif
};

