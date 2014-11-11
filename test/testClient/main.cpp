#include <iostream>
#include "SoundBuffer.h"


int			testSoundBuffer()
{
	SoundBuffer		buff;
	int				size;
	char			tmp[BUFFER_SIZE + 1];

	std::cout << "Testing SoundBufffer ..." << std::endl;
	buff.print();

	std::cout << "pushing \"HELLO\" ..." << std::endl;
	buff.pushEncriptedSound("HELLO", 5);
	buff.print();
	std::cout << std::endl;

	std::cout << "pushing \"LOL\" ..." << std::endl;
	buff.pushEncriptedSound("LOL", 3);
	buff.print();
	std::cout << std::endl;

	std::cout << "geting \"HELLOL\" ..." << std::endl;
	size = buff.cpyEncriptedSound(tmp, 6);
	tmp[size] = 0;
	buff.print();
	std::cout << "Tmp contains : " << tmp << std::endl;
	std::cout << std::endl;

	std::cout << "pushing \"012345678\" ..." << std::endl;
	buff.pushEncriptedSound("012345678", 9);
	buff.print();
	std::cout << std::endl;

	std::cout << "pushing \"BG\" ..." << std::endl;
	buff.pushEncriptedSound("BG", 2);
	buff.print();
	std::cout << std::endl;

	std::cout << "geting the all buffer ..." << std::endl;
	size = buff.cpyEncriptedSound(tmp, BUFFER_SIZE);
	tmp[size] = 0;
	buff.print();
	std::cout << "Tmp contains : " << tmp << std::endl;
	std::cout << std::endl;

	std::cout << "geting the all buffer ..." << std::endl;
	size = buff.cpyEncriptedSound(tmp, BUFFER_SIZE);
	tmp[size] = 0;
	buff.print();
	std::cout << "Tmp contains : " << tmp << std::endl;
	std::cout << std::endl;


	std::cout << "End of tests, enter a number to close the program" << std::endl;
	std::cin >> size;
}


int			main()
{
	testSoundBuffer();
}
