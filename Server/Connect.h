#pragma once

#include "pch.h"
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

class Connect
{
	TcpSocket socket;
	Storage& storage;
	enum class ErrType{BadFormat,KeyNotExist, KeyAllreadyExist};
public:
	Connect(boost::asio::io_service&, Storage& storage);
	~Connect();
	TcpSocket& Socket();
	void Run();
private:
	std::string Read();
	boost::property_tree::ptree Processing(std::string&);
	boost::property_tree::ptree Error(ErrType);
	boost::property_tree::ptree TryRead(boost::property_tree::ptree&);
	boost::property_tree::ptree TryWrite(boost::property_tree::ptree&);
	boost::property_tree::ptree Ok();
	boost::property_tree::ptree MakeGoodRead(std::string);
	bool Write(boost::property_tree::ptree&);
};

