#include <exception>
#include <iostream>
#include <string.h>
#include "SoundBuffer.h"

SoundBuffer::SoundBuffer()
{
    this->_begin = 0;
    this->_size = 0;
}

SoundBuffer::~SoundBuffer()
{
}

int         SoundBuffer::pushEncriptedSound(unsigned char const *sound, int size)
{
    int     i;
    int     e;
    unsigned char    *dest;
    int     newSize;

    if (size > SOUND_SIZE_PER_SEC)
        throw std::exception();
    newSize = MIN(SOUND_SIZE_PER_SEC, this->_size + size);
    e = (this->_begin + this->_size) % SOUND_SIZE_PER_SEC;
    if (newSize == SOUND_SIZE_PER_SEC)
	{
        this->_begin = (this->_begin + this->_size + size) % SOUND_SIZE_PER_SEC;
		std::cout << "SoundBuffer : override buffer ..." << std::endl;
	}
	dest = &this->_buff[e];
    i = MIN(SOUND_SIZE_PER_SEC - e, size);
    memcpy(dest, sound, i);
    if (i != size)
        memcpy(this->_buff, sound + i, size - i);
    this->_size = newSize;
    return (this->_size);
}

int        SoundBuffer::cpyEncriptedSound(unsigned char *dest, int len)
{
    int     i;
    int     realSize;

    if (len > SOUND_SIZE_PER_SEC )
        throw std::exception();
    realSize = MIN(this->_size, len);
    i = MIN(SOUND_SIZE_PER_SEC - this->_begin, realSize);
    memcpy(dest, &this->_buff[this->_begin], i);
    if (i != realSize)
        memcpy(dest + i, this->_buff, realSize - i);
    this->_begin = (this->_begin + realSize) % SOUND_SIZE_PER_SEC;
    this->_size -= realSize;
    return (realSize);
}

void        SoundBuffer::print() const
{
    int     i;

    i = 0;
    std::cout << "SoundBuffer : size = " << this->_size << " 0x" << std::hex;
    for (; i < this->_size; ++i)
        std::cout << this->_buff[(this->_begin + i) % SOUND_SIZE_PER_SEC];
    std::cout << std::dec << std::endl;
}

int             SoundBuffer::getSize() const
{
    return (this->_size);
}
