#pragma once
#include "SoundConstant.h"

#define MIN(x, y) (((x) > (y)) ? y : x)

class SoundBuffer
{
public:
    SoundBuffer();
    ~SoundBuffer();

    int     pushEncriptedSound(const unsigned char *, int);
    int     cpyEncriptedSound(unsigned char *, int);

    void    print() const;
    int     getSize() const;

private:
    unsigned char    _buff[SOUND_SIZE_PER_SEC];
    int     _begin;
    int     _size;
};
