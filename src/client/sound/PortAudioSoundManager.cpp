#include <iostream>
#include "PortAudioSoundManager.h"

PortAudioSoundManager::PortAudioSoundManager(SoundContainer &container, ICall *call) :
	_container(container)
{
	this->_stream = 0;
    this->_call = call;
	if (Pa_Initialize() != paNoError)
		throw std::exception();
}

PortAudioSoundManager::~PortAudioSoundManager()
{
	Pa_Terminate();
}

int				PortAudioSoundManager::start()
{
	int			err;

	this->_inputParam.device = Pa_GetDefaultInputDevice();
	if (this->_inputParam.device == paNoDevice)
		throw std::exception();
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
		portaudio_callback,
		this);

	if (err != paNoError)
		throw std::exception();

	err = Pa_StartStream(this->_stream);
	if (err != paNoError)
		throw std::exception();
	std::cout << "PortAudioSoundManager : Stream started" << std::endl;
	return (1);
}

int				PortAudioSoundManager::callback(const void *input, void *output, unsigned long framePerBuffer)
{
	//PushInput
	/* If mute */
	this->_container.lockInput();
	this->_mixer.encryptInput((const short *)input, framePerBuffer, this->_container.getInput());
    this->_call->sendInput();
	this->_container.unlockInput();

	//Merge output
	/* If silence */
	this->_container.lockVoices();
	this->_mixer.mergeVoices((short*)output, framePerBuffer, this->_container.getVoices(), this->_container.getProducerCount());
	this->_container.unlockVoices();

	//Check end
	return (paContinue);
}

int		portaudio_callback(const void *input, void *output, unsigned long framePerBuffer, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void *param)
{
	PortAudioSoundManager *manager;

	manager = (PortAudioSoundManager*)(param);
	return (manager->callback(input, output, framePerBuffer));
}

