#pragma once

#include <string>

class FileSerializer
{
public:
	FileSerializer();
	FileSerializer(std::string const&);

	~FileSerializer();

public:
	void					chooseFile(std::string const&);
	std::string				readFromFile() const;
	void					writeToFile(std::string const&) const;

private:
	std::string _filename;
};

