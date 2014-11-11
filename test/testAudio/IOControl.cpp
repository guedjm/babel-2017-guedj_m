#include "IOControl.h"
#include "MyMutex.hpp"

IOControl::IOControl()
{
	int		err;

	this->_stream = 0;
	this->_inputIndex = 0;
	this->_outputSize = 0;
	this->_inputMut = new myMutex();
	this->_outputMut = new myMutex();
	memset(this->_inputBuff, 0, TMP_BUFF_SIZE);
	memset(this->_outputBuff, 0, TMP_BUFF_SIZE);
	err = Pa_Initialize();
	if (err != paNoError || this->_outputMut == NULL || this->_inputMut == NULL)
		throw std::exception();
}

IOControl::~IOControl()
{
	Pa_Terminate();
}

//FULL
int			IOControl::startFull()
{
	int		err;

	this->_inputParam.device = Pa_GetDefaultInputDevice();
	if (this->_inputParam.device == paNoDevice)
	  {
	    std::cout << "C'est ici" << std::endl;
		throw std::exception();
	  }
	this->_inputParam.channelCount = NUM_CHANNELS;
	this->_inputParam.sampleFormat = PA_TYPE;
	this->_inputParam.suggestedLatency = Pa_GetDeviceInfo(this->_inputParam.device)->defaultLowInputLatency;
	this->_inputParam.hostApiSpecificStreamInfo = NULL;

	this->_outputParam.device = Pa_GetDefaultOutputDevice();
	if (this->_outputParam.device == paNoDevice)
		throw std::exception();
	this->_outputParam.channelCount = NUM_CHANNELS;
	this->_outputParam.sampleFormat = PA_TYPE;
	this->_outputParam.suggestedLatency = Pa_GetDeviceInfo(this->_outputParam.device)->defaultLowOutputLatency;
	this->_outputParam.hostApiSpecificStreamInfo = NULL;

	err = Pa_OpenStream(&this->_stream,
		&this->_inputParam,
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
	std::cout << "Running ..." << std::endl;

	return (1);
}

int			IOCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void *userData)
{
	IOControl *ctrl = (IOControl*)(userData);
	return (ctrl->callback(inputBuffer, outputBuffer, framesPerBuffer));
}

int			IOControl::callback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer)
{
	int		err;

	//Encode Input and store it in _input
	unsigned char *dest = &this->_inputBuff[this->_inputIndex];
	this->_inputMut->lock();
	err = this->_codec.encode((const short*)inputBuffer, dest, framesPerBuffer, TMP_BUFF_SIZE - this->_inputIndex);
	this->_inputIndex += err;
	this->_inputMut->unLock();

	//decode output
	this->_outputMut->lock();
	err = this->_codec.decode(this->_outputBuff, (short*)outputBuffer, this->_outputSize, framesPerBuffer);
	this->_outputSize = 0;
	memset(this->_outputBuff, 0, TMP_BUFF_SIZE);
	this->_outputMut->unLock();
	return (paContinue);
}

int			IOControl::copyInput(unsigned char *dest, unsigned int size)
{
	unsigned int		realSize;

	realSize = (size < this->_inputIndex) ? size : this->_inputIndex;
	if (realSize != 0)
	{
		this->_inputMut->lock();
		memcpy(dest, this->_inputBuff, realSize);
		this->_inputIndex -= realSize;
		this->_inputMut->unLock();
	}
	return (realSize);
}

int			IOControl::enqueueOutput(unsigned char *buff, unsigned int size)
{
	unsigned int	realSize;

	realSize = (size > TMP_BUFF_SIZE - this->_outputSize) ? TMP_BUFF_SIZE - this->_outputSize : size;
	this->_outputMut->lock();
	memcpy(&this->_outputBuff[this->_outputSize], buff, size);
	this->_outputSize += size;
	this->_outputMut->unLock();
	return (realSize);
}


