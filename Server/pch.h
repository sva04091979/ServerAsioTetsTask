#pragma once

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include "Storage.h"

constexpr size_t gBuffSize = 1024;

class Connect;

using ConnectPtr = boost::shared_ptr<Connect>;
using SocketPtr = boost::shared_ptr<boost::asio::ip::tcp::socket>;
using TcpSocket = boost::asio::ip::tcp::socket;
