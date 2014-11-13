#pragma once

#include "ICall.h"
#include "portaudio.h"
#include "ISoundManager.h"
#include "SoundParam.h"
#include "SoundContainer.h"
#include "SoundMixer.h"

int		portaudio_callback(const void *, void *, unsigned long, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void *);

class PortAudioSoundManager : public ISoundManager
{
public:
    PortAudioSoundManager(SoundContainer&, ICall *);
	virtual ~PortAudioSoundManager();

	virtual int	start();

	int	callback(const void *, void *, unsigned long);

private:
	PaStream			*_stream;
	PaStreamParameters	_inputParam;
	PaStreamParameters	_outputParam;

    ICall               *_call;
	SoundContainer		&_container;
	SoundMixer			_mixer;
};
