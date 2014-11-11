#pragma once

#include "opus.h"
#include "ICodecManager.h"

class OpusCodecManager : public ICodecManager
{
public:
	OpusCodecManager();
	virtual ~OpusCodecManager();

	virtual int encode(const short *, unsigned char *, unsigned long, int);
	virtual int		decode(const unsigned char *, short *, int, int);

private:
	OpusEncoder		*_encoder;
	OpusDecoder		*_decoder;
};
