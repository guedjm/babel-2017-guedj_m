#pragma once

#include <map>
#include "IMutex.h"
#include "ICodecManager.h"
#include "SoundBuffer.h"

class SoundContainer
{
public:
	SoundContainer();
	~SoundContainer();

	int							addProducer(int);
	void						removeProducer(int);
	int							getProducerCount() const;

	void						pushEncriptedOutputSound(int, const char *, int);

	void						lockVoices();
	std::map<int, SoundBuffer>	&getVoices();
	void						unlockVoices();

	int							pushInputSound(char *, int);

	void						lockInput();
	SoundBuffer					&getInput();
	void						unlockInput();

private:
	IMutex						*_lockInput;
	IMutex						*_lockOutput;

	int							_producerCount;
	std::map<int, SoundBuffer>	_voices;
	SoundBuffer					_input;
};
