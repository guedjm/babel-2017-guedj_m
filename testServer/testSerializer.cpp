#include "Serializer.hpp"
#include <iostream>
#include <sstream>

void	testSerializeString()
{
	std::string s("Hello");
	std::string r = Serializer::serialize<std::string>(s);
	std::cout << "'" << s << "' serialized becomes: '" << r << "'." << std::endl << "Size: " << ((int)r[0]) << std::endl;
	std::istringstream iss(r);
	std::string new_s = Serializer::deserialize<std::string>(iss);
	std::cout << "Deserializing again: '" << new_s << "'" << std::endl;
}

void	testSerializeInt()
{
	int a = 61176;
	std::string r = Serializer::serialize<int>(a);
	std::cout << "'" << a << "' serialized becomes: '" << r << "'." << std::endl;
	std::istringstream iss(r);
	int new_a = Serializer::deserialize<int>(iss);
	std::cout << "Deserializing again: '" << new_a << "'" << std::endl;
}

void	testSerializeIntList()
{
	std::list<int> list;
	list.push_back(117);
	list.push_back(1);
	list.push_back(2);
	list.push_back(3);
	std::string r = Serializer::serialize< std::list<int> >(list);
	std::cout << "List(117, 1, 2, 3) serialized becomes: '" << r << "'." << std::endl;
	std::istringstream iss(r);
	std::list<int> newList = Serializer::deserialize< std::list<int> >(iss);
	std::cout << "Displaying deserialized: Size=" << newList.size() << ", Values = [";
	for (std::list<int>::iterator it = newList.begin(); it != newList.end(); ++it)
		std::cout << *it << ", ";
	std::cout << "]" << std::endl;
}

void	testSerializeStringList()
{
	std::list<std::string> list;
	list.push_back("Hello");
	list.push_back("World");
	list.push_back("!");
	list.push_back(":)");
	std::string r = Serializer::serialize< std::list<std::string> >(list);
	std::cout << "List(\"Hello\", \"World\", \"!\", \":)\") serialized becomes: '" << r << "'." << std::endl;
	std::istringstream iss(r);
	std::list<std::string> newList = Serializer::deserialize< std::list<std::string> >(iss);
	std::cout << "Displaying deserialized: Size=" << newList.size() << ", Values = [";
	for (std::list<std::string>::iterator it = newList.begin(); it != newList.end(); ++it)
		std::cout << *it << ", ";
	std::cout << "]" << std::endl;
}

void	testSerializeStringCharPair()
{
	std::pair<std::string, char> p("Spartan", 117);
	std::string r = Serializer::serialize< std::pair<std::string, char> >(p);
	std::cout << "pair(\"Spartan\", 117) serialized becomes: '" << r << "'." << std::endl;
	std::istringstream iss(r);
	std::pair<std::string, char> new_p = Serializer::deserialize< std::pair<std::string, char> >(iss);
	std::cout << "Deserializing again: pair(" << new_p.first << ", " << (int)new_p.second << ")" << std::endl;
}

void	testSerializeStringCharPairList()
{
	std::list<std::pair<std::string, char>> list;
	list.push_back(std::pair<std::string, char>("Spartan", 117));
	list.push_back(std::pair<std::string, char>("Xbox", 1));
	list.push_back(std::pair<std::string, char>("Halo", 5));
	std::string r = Serializer::serialize<std::list<std::pair<std::string, char>>>(list);
	std::cout << "List: [(\"Spartan\", 117); (\"Xbox\", 1); (\"Halo\", 5)] serialized becomes: '" << r << "'." << std::endl;
	std::istringstream iss(r);
	std::list<std::pair<std::string, char>> new_list = Serializer::deserialize<std::list<std::pair<std::string, char>>>(iss);
	std::cout << "Displaying deserialized: Size=" << new_list.size() << ", Values = [";
	for (std::list<std::pair<std::string, char>>::iterator it = new_list.begin(); it != new_list.end(); ++it)
		std::cout << "(" << (*it).first << ", " << (int)(*it).second << "), ";
	std::cout << "]" << std::endl;
}
