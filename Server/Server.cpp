// Server.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "pch.h"
#include <iostream>
#include <boost/thread.hpp>
#include "Connect.h"

using namespace boost::asio;

void Session(ConnectPtr connect) {
	std::cout << "client has connected" << std::endl;
	connect->Run();
	std::cout << "client has disconnected" << std::endl;
}

void Listener(io_service& service,Storage& storage) {
	ip::tcp::acceptor acceptor(service,ip::tcp::endpoint(ip::tcp::v4(),8001));
	boost::thread_group sessions;
	while (true) {
		std::cout << "wait connect" << std::endl;
		ConnectPtr connect(new Connect(service,storage));
		acceptor.accept(connect->Socket());
		sessions.create_thread(boost::bind(Session,connect));
	}
	sessions.join_all();
}

int main()
{
	Storage storage;
	boost::asio::io_service service;
	Listener(service,storage);
}
