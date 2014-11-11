#pragma once

#include "portaudio.h"
#include "ISoundManager.h"
#include "SoundParam.h"
#include "SoundContainer.h"
#include "SoundMixer.h"

int		portaudio_callback(const void *, void *, unsigned long, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void *);

class PortAudioSoundManager : public ISoundManager
{
public:
	PortAudioSoundManager(SoundParam const &, SoundContainer&);
	virtual ~PortAudioSoundManager();

	virtual int	start();

	int	callback(const void *, void *, unsigned long);

private:
	PaStream			*_stream;
	PaStreamParameters	_inputParam;
	PaStreamParameters	_outputParam;

	SoundParam const	&_param;
	SoundContainer		&_container;
	SoundMixer			_mixer;
};