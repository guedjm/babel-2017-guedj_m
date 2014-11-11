#include "OpusCodecManager.h"
#include "SoundMixer.h"

SoundMixer::SoundMixer()
{
	this->_codec = new OpusCodecManager();
	if (this->_codec == 0)
		throw std::exception();
}

SoundMixer::~SoundMixer()
{
	delete this->_codec;
}

int			SoundMixer::mergeVoices(short *dest, int framePerBuffer, std::map<int, SoundBuffer> &voices, int voicesCount)
{
	int				i;

	memset(this->_tmpF, 0, SOUND_SIZE_PER_SEC * sizeof(float));
	std::map<int, SoundBuffer>::iterator it = voices.begin();
	while (it != voices.end())
	{
		i = it->second.cpyEncriptedSound(this->_tmp, framePerBuffer * NUM_CHANNELS);
		if (i < framePerBuffer * NUM_CHANNELS)
			memset(this->_tmp, 0, framePerBuffer * NUM_CHANNELS - i);
		for (i = 0; i < framePerBuffer * NUM_CHANNELS; ++i)
			this->_tmpF[i] += (this->_tmp[i] / voicesCount);
		++it;
	}
	for (i = 0; i < framePerBuffer * NUM_CHANNELS; ++i)
		dest[i] = (short)this->_tmpF[i];
	return (1);
}

int			SoundMixer::encryptInput(const short *input, int size, SoundBuffer &buff)
{
	int		esize;

	esize = this->_codec->encode(input, this->_tmp, size, FRAMES_PER_BUFFER * NUM_CHANNELS);
	buff.cpyEncriptedSound(this->_tmp, esize);
	return (esize);
}

