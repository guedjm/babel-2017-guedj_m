#include <exception>
#include "SoundConstant.h"
#include "OpusCodecManager.h"

OpusCodecManager::OpusCodecManager()
{
	int err;

	this->_encoder = opus_encoder_create(SAMPLE_RATE, NUM_CHANNELS, OPUS_APPLICATION_VOIP, &err);
	if (err != OPUS_OK)
		throw std::exception();
	this->_decoder = opus_decoder_create(SAMPLE_RATE, NUM_CHANNELS, &err);
	if (err != OPUS_OK)
		throw std::exception();
}

OpusCodecManager::~OpusCodecManager()
{
	opus_encoder_destroy(this->_encoder);
	opus_decoder_destroy(this->_decoder);
}

int			OpusCodecManager::encode(const short *src, unsigned char *dest, unsigned long size, int freeSpace)
{
	int		err;

	err = opus_encode(this->_encoder, src, size, dest, freeSpace);
	if (err < 0)
		throw std::exception();
	return (err);
}

int			OpusCodecManager::decode(const unsigned char *src, short *dest, int srclen, int destlen)
{
	int		err;

	err = opus_decode(this->_decoder, src, srclen, dest, destlen, 0);
	if (err < 0)
		throw std::exception();
	return (err);
}
