#pragma once
#include <map>
#include <string>
#include <boost/thread/mutex.hpp>
#include <boost/optional.hpp>

class Storage
{
	std::map<std::string, std::string> data;
	boost::mutex mu;
public:
	bool Add(std::string&, std::string&);
	boost::optional<std::string> Get(std::string&);
};

