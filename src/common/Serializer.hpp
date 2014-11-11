#pragma once

#include <sstream>
#include <utility>
#include <string>
#include <list>


class Serializer
{
public:
	~Serializer();

	template <class T> static std::string serialize(T const&);
	template <> static std::string serialize <std::string>(std::string const&);

	template <class T> static T deserialize(std::istringstream&);
	template <> static std::string deserialize <std::string>(std::istringstream&);

private:
	Serializer();

};


// String Specialization
template <> static std::string Serializer::serialize <std::string>(std::string const& data)
{
	return ((char)(data.length())) + data;
}

template <> static std::string Serializer::deserialize <std::string>(std::istringstream& iss)
{
	int length = iss.get();
	char* buff = new char[length];
	iss.read(buff, length);
	std::string r("");
	r.append(buff, length);
	delete[] buff;
	return r;
}


// Generic Template
template <class T> std::string Serializer::serialize(T const& data)
{
	return Serializer_Impl<T>::serialize(data);
}

template <class T> T Serializer::deserialize(std::istringstream& iss)
{
	return Serializer_Impl<T>::deserialize(iss);
}



// Implementation: Generic Template
template <typename T> struct Serializer_Impl
{
	static std::string serialize(T const& data)
	{
		const char* buff = reinterpret_cast<const char*>(&data);
		std::string r;
		r.append(buff, sizeof(T));
		return r;
	}

	static T deserialize(std::istringstream& iss)
	{
		char* buff = new char[sizeof(T)];
		iss.read(buff, sizeof(T));
		T tmp = *(reinterpret_cast<T*>(buff));
		T data(tmp);
		delete[] buff;
		return data;
	}
};


// Implementation: Generic List Template
template <typename T> struct Serializer_Impl<std::list<T>>
{
	static std::string serialize(std::list<T> const& data)
	{
		std::string r = Serializer::serialize<int>(data.size());
		for (std::list<T>::const_iterator it = data.begin(); it != data.end(); ++it)
		{
			std::string tmp = Serializer::serialize<T>(*it);
			r.append(tmp);
		}
		return r;
	}

	static std::list<T> deserialize(std::istringstream& iss)
	{
		std::list<T> r;
		int length = Serializer::deserialize<int>(iss);
		for (int i = 0; i < length; ++i)
		{
			T elem = Serializer::deserialize<T>(iss);
			r.push_back(elem);
		}
		return r;
	}
};


// Implementation: Generic Pair Template
template <typename U, typename V> struct Serializer_Impl< std::pair<U, V> >
{
	static std::string serialize(std::pair<U, V> const& data)
	{		
		std::string r = Serializer::serialize<U>(data.first);
		r += Serializer::serialize<V>(data.second);
		return r;
	}

	static std::pair<U, V> deserialize(std::istringstream& iss)
	{
		U first = Serializer::deserialize<U>(iss);
		V second = Serializer::deserialize<V>(iss);
		std::pair<U, V> r(first, second);
		return r;
	}
};


