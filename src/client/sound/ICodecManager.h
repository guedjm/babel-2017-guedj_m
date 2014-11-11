#pragma once

class	ICodecManager
{
public:
	virtual ~ICodecManager() {}

	virtual int		encode(const short *, unsigned char *, unsigned long, int) = 0;
	virtual int		decode(const unsigned char *, short *, int, int) = 0;
};