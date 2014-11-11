#pragma once

#include <iostream>
#include <string.h>
#include <exception>
#include "portaudio.h"
#include "constant.h"
#include "Codec.h"
#include "IMutex.h"


int			IOCallback(const void *, void *, unsigned long, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void *);

class IOControl
{
public:
	IOControl();
	~IOControl();

	int		startFull();
	int		callback(const void *, void *, unsigned long);
	int		copyInput(unsigned char *, unsigned int);
	int		enqueueOutput(unsigned char *, unsigned int);

private:

	PaStream			*_stream;
	PaStreamParameters	_inputParam;
	PaStreamParameters	_outputParam;
	Codec				_codec;

	IMutex				*_inputMut;
	unsigned char		_inputBuff[TMP_BUFF_SIZE];
	unsigned int		_inputIndex;

	IMutex				*_outputMut;
	unsigned char		_outputBuff[TMP_BUFF_SIZE];
	unsigned int		_outputSize;
};

