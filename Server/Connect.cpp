#include "Connect.h"
#include <iostream>

using namespace boost::asio;
namespace pt = boost::property_tree;

Connect::Connect(boost::asio::io_service& service,Storage& _storage):
	socket(service),storage(_storage){}

Connect::~Connect() {
	boost::system::error_code error;
	socket.close(error);
}

TcpSocket& Connect::Socket() { return socket; }

void Connect::Run() {
	while (true) {
		auto mess = Read();
		if (!mess.length()) break;
		pt::ptree answer;
		try {
			answer = Processing(mess);
		}
		catch (...) {
			answer = Error(ErrType::BadFormat);
		}
		if (!Write(answer)) break;
	}
}

std::string Connect::Read() {
	std::string mess;
	streambuf buff;
	boost::system::error_code error;
	read_until(socket, buff, '\0', error);
	if (!error) {
		std::istream is(&buff);
		std::getline(is, mess,'\0');
	}
	return mess;
}

bool Connect::Write(pt::ptree& mess) {
	boost::system::error_code error;
	std::stringstream it;
	pt::write_json(it, mess);
	socket.write_some(buffer(it.str()),error);
	return !error;
}

pt::ptree Connect::Processing(std::string& mess) {
	pt::ptree root;
	std::stringstream it(mess);
	pt::read_json(it,root);
	auto request=root.get<std::string>("request");
	if (request == "read") return TryRead(root);
	else if (request == "write") return TryWrite(root);
	else return Error(ErrType::BadFormat);
}

pt::ptree Connect::TryRead(pt::ptree& root) {
	auto key = root.get<std::string>("key");
	auto val=storage.Get(key);
	return !val ? Error(ErrType::KeyNotExist) : MakeGoodRead(*val);
}

pt::ptree Connect::TryWrite(pt::ptree& root) {
	auto key= root.get<std::string>("key");
	auto value= root.get<std::string>("value");
	return storage.Add(key, value) ? Ok() : Error(ErrType::KeyAllreadyExist);
}

pt::ptree Connect::Ok() {
	pt::ptree ret;
	ret.put("status", "ok");
	return ret;
}

pt::ptree Connect::MakeGoodRead(std::string value) {
	pt::ptree ret = Ok();
	ret.put("value", value);
	return ret;
}

pt::ptree Connect::Error(ErrType error) {
	pt::ptree ret;
	ret.put("status", "error");
	switch (error) {
	case ErrType::BadFormat:
		ret.put("description", "bad format");
		break;
	case ErrType::KeyNotExist:
		ret.put("description", "key not exist");
		break;
	case ErrType::KeyAllreadyExist:
		ret.put("description", "key allready exist");
		break;
	}
	return ret;
}