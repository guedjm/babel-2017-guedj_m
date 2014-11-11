#include <exception>
#include <iostream>
#include "IOControl.h"

IOControl::IOControl()
{
	int		err;

	this->_stream = 0;

#ifdef RECORD
	this->_inputIndex = 0;
	memset(this->_inputBuff, 0, RECORDING_BUFF_SIZE);
#else

	this->_outputIndex = 0;
	memset(this->_outputBuff, 0, RECORDING_BUFF_SIZE);
	this->_f1.open(FILENAME1, std::ios::in | std::ios::binary);
	this->_f2.open(FILENAME2, std::ios::in | std::ios::binary);

#endif

	err = Pa_Initialize();
	if (err != paNoError)
		throw std::exception();
}


IOControl::~IOControl()
{
	Pa_Terminate();
}

bool	IOControl::streamEnded() const
{
	if (!Pa_IsStreamActive(this->_stream))
	{
		Pa_CloseStream(this->_stream);
		return (true);
	}
	return (false);
}

#ifdef RECORD
int		IOControl::startRecord()
{
	int		err;

	this->_inputParam.device = Pa_GetDefaultInputDevice();
	if (this->_inputParam.device == paNoDevice)
		throw std::exception();
	this->_inputParam.channelCount = NUM_CHANNELS;
	this->_inputParam.sampleFormat = PA_TYPE;
	this->_inputParam.suggestedLatency = Pa_GetDeviceInfo(this->_inputParam.device)->defaultLowInputLatency;
	this->_inputParam.hostApiSpecificStreamInfo = NULL;

	err = Pa_OpenStream(&this->_stream,
		&this->_inputParam,
		NULL,
		SAMPLE_RATE,
		FRAMES_PER_BUFFER,
		paClipOff,
		IOCallback,
		this);

	if (err != paNoError)
		throw std::exception();

	err = Pa_StartStream(this->_stream);
	if (err != paNoError)
		throw std::exception();

	std::cout << "Start recording" << std::endl;

	return (1);
}

int		IOControl::recordCallback(const void *input, unsigned long framePerBuff)
{
	int				ret;
	short			*dest = &this->_inputBuff[this->_inputIndex];
	unsigned int	size;

	if ((RECORDING_BUFF_SIZE - this->_inputIndex) > (framePerBuff * NUM_CHANNELS))
	{
		ret = paContinue;
		size = framePerBuff * NUM_CHANNELS;
	}
	else
	{
		ret = paComplete;
		size = (RECORDING_BUFF_SIZE - this->_inputIndex);
	}
	memcpy(dest, input, size * sizeof(short));
	this->_inputIndex += size;
	return (ret);
}

int		IOControl::copyInput(short *dest)
{
	memcpy(dest, this->_inputBuff, RECORDING_BUFF_SIZE * sizeof(short));
	return (0);
}
#else

int		IOControl::startPlay()
{
	int	err;

	this->_outputParam.device = Pa_GetDefaultOutputDevice();
	if (this->_outputParam.device == paNoDevice)
		throw std::exception();
	this->_outputParam.channelCount = 2;
	this->_outputParam.sampleFormat = PA_TYPE;
	this->_outputParam.suggestedLatency = Pa_GetDeviceInfo(this->_outputParam.device)->defaultLowOutputLatency;
	this->_outputParam.hostApiSpecificStreamInfo = NULL;

	err = Pa_OpenStream(&this->_stream,
		NULL,
		&this->_outputParam,
		SAMPLE_RATE,
		FRAMES_PER_BUFFER,
		paClipOff,
		IOCallback,
		this);

	if (err != paNoError)
		throw std::exception();

	err = Pa_StartStream(this->_stream);
	if (err != paNoError)
		throw std::exception();
	std::cout << "Playing ..." << std::endl;
}

int		IOControl::playCallback(void *output, unsigned long framePerBuffer)
{
	int				ret;
	short			*dest = (short*)output;
	short			buff1[FRAMES_PER_BUFFER * 2];
	short			buff2[FRAMES_PER_BUFFER * 2];

	memset(dest, 0, framePerBuffer * NUM_CHANNELS * sizeof(short));
	this->_f1.read((char*)buff1, FRAMES_PER_BUFFER * NUM_CHANNELS * sizeof(short));
	this->_f2.read((char*)buff2, FRAMES_PER_BUFFER * NUM_CHANNELS * sizeof(short));
	if (!this->_f1.eof())
		ret = paContinue;
	else
		ret = paComplete;
	for (int i = 0; i < FRAMES_PER_BUFFER * NUM_CHANNELS; ++i)
		dest[i] = (buff1[i] + buff2[i]) / 2;
	return (ret);
}

int		IOControl::copyOutput(short *output)
{
	memcpy(this->_outputBuff, output, RECORDING_BUFF_SIZE * sizeof(short));
	return (1);
}

#endif

int			IOCallback(const void *input, void *output, unsigned long framePerBuff, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void *param)
{
	IOControl *ctrl = static_cast<IOControl*>(param);

#ifdef RECORD
	return (ctrl->recordCallback(input, framePerBuff));
#else
	return (ctrl->playCallback(output, framePerBuff));
#endif
}

