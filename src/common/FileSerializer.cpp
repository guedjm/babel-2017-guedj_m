#include <fstream>
#include <stdexcept>
#include "FileSerializer.h"


FileSerializer::FileSerializer()
{
}

FileSerializer::FileSerializer(std::string const& filename) : _filename(filename)
{
}

FileSerializer::~FileSerializer()
{
}


std::string			FileSerializer::readFromFile() const
{
	std::string r = "";
	std::ifstream file(this->_filename, std::ios::in | std::ios::binary | std::ios::ate);
	if (file.is_open() == false)
		return r;
	std::streampos size = file.tellg();
	if (size > 0)
	{
		char* buff = new char[size];
		file.seekg(0, std::ios::beg);
		file.read(buff, size);
		
		r.append(buff, size);
		delete[] buff;
	}
	file.close();
	return r;
}

void					FileSerializer::writeToFile(std::string const& data) const
{
	if (this->_filename.size() == 0)
		throw std::runtime_error("Empty filename for output operations");
	std::ofstream file(this->_filename, std::ios::out | std::ios::binary | std::ios::trunc);
	if (file.is_open() == false)
		throw std::runtime_error(std::string("Could not open file '") + this->_filename + std::string("' for writing operations"));
	file.write(data.c_str(), data.size());
	file.close();
}


