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

    if (size > BUFFER_SIZE)
        throw std::exception();
    newSize = MIN(BUFFER_SIZE, this->_size + size);
    e = (this->_begin + this->_size) % BUFFER_SIZE;
	if (newSize == BUFFER_SIZE)
	{
		this->_begin = (this->_begin + this->_size + size) % BUFFER_SIZE;
		std::cout << "SoundBuffer : override buffer ..." << std::endl;
	}
	dest = &this->_buff[e];
    i = MIN(BUFFER_SIZE - e, size);
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

    if (len > BUFFER_SIZE )
        throw std::exception();
    realSize = MIN(this->_size, len);
    i = MIN(BUFFER_SIZE - this->_begin, realSize);
    memcpy(dest, &this->_buff[this->_begin], i);
    if (i != realSize)
        memcpy(dest + i, this->_buff, realSize - i);
    this->_begin = (this->_begin + realSize) % BUFFER_SIZE;
    this->_size -= realSize;
    return (realSize);
}

void        SoundBuffer::print() const
{
    int     i;

    i = 0;
    std::cout << "SoundBuffer : size = " << this->_size << " 0x" << std::hex;
    for (; i < this->_size; ++i)
        std::cout << this->_buff[(this->_begin + i) % BUFFER_SIZE];
    std::cout << std::dec << std::endl;
}

int             SoundBuffer::getSize() const
{
    return (this->_size);
}
