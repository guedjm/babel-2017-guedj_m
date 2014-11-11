#pragma once

#include <map>
#include "SoundConstant.h"
#include "SoundBuffer.h"
#include "ICodecManager.h"

class SoundMixer
{
public:
	SoundMixer();
	~SoundMixer();

	int			mergeVoices(short *, int, std::map<int, SoundBuffer>&, int);
	int			encryptInput(const short *, int, SoundBuffer&);

private:
	float			_tmpF[SOUND_SIZE_PER_SEC];
	unsigned char	_tmp[FRAMES_PER_BUFFER * NUM_CHANNELS];
	ICodecManager	*_codec;
};
