#pragma once
#include "opus.h"
#include "constant.h"

class Codec
{
public:
	Codec();
	~Codec();

	int encode(const short *, unsigned char *, unsigned long, int);
	int decode(const unsigned char *, short *, int, int);

private:
	OpusEncoder		*_encoder;
	OpusDecoder		*_decoder;
};

