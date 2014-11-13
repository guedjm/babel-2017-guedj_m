#include "myMutex.h"
#include "SoundContainer.h"

SoundContainer::SoundContainer()
{
	this->_producerCount = 0;
	this->_lockInput = new myMutex();
	this->_lockOutput = new myMutex();
	if (this->_lockInput == 0 || this->_lockOutput == 0)
		throw std::exception();
}

SoundContainer::~SoundContainer()
{
	delete this->_lockInput;
	delete this->_lockOutput;
}

int							SoundContainer::addProducer(int id)
{
	this->_lockOutput->lock();
	++this->_producerCount;
	std::map<int, SoundBuffer>::iterator it = this->_voices.find(id);
	if (it != this->_voices.end())
	{
		throw std::exception();
		this->_lockOutput->unLock();
	}
	this->_voices[id] = SoundBuffer();
	this->_lockOutput->unLock();
	return (1);
}

void						SoundContainer::removeProducer(int id)
{
	this->_lockOutput->lock();
	--this->_producerCount;
	std::map<int, SoundBuffer>::iterator it = this->_voices.find(id);
	if (it == this->_voices.end())
	{
		throw std::exception();
		this->_lockOutput->unLock();
	}
	this->_voices.erase(it);
	this->_lockOutput->unLock();
}

int							SoundContainer::getProducerCount() const
{
	return (this->_producerCount);
}

void						SoundContainer::pushEncriptedOutputSound(int id, const unsigned char *voice, int size)
{
	this->_lockOutput->lock();
	std::map<int, SoundBuffer>::iterator it = this->_voices.find(id);
	if (it != this->_voices.end())
	{
		throw std::exception();
		this->_lockOutput->unLock();
	}
	this->_voices[id].pushEncriptedSound(voice, size);
	this->_lockOutput->unLock();
}

void						SoundContainer::lockVoices()
{
	this->_lockOutput->lock();
}

std::map<int, SoundBuffer>	&SoundContainer::getVoices()
{
	return (this->_voices);
}

void						SoundContainer::unlockVoices()
{
	this->_lockOutput->unLock();
}

int							SoundContainer::pushInputSound(unsigned char *buff, int size)
{
	int						ret;

	this->_lockInput->lock();
	ret = this->_input.cpyEncriptedSound(buff, size);
	this->_lockOutput->unLock();
	return (ret);
}

void						SoundContainer::lockInput()
{
	this->_lockOutput->lock();
}

SoundBuffer					&SoundContainer::getInput()
{
	return (this->_input);
}

void						SoundContainer::unlockInput()
{
	this->_lockOutput->unLock();
}
