#include "Storage.h"

using namespace std;

bool Storage::Add(string& key, string& value) {
	boost::mutex::scoped_lock lk(mu);
	return data.insert({ key,value }).second;
}

boost::optional<string> Storage::Get(string& key) {
	boost::mutex::scoped_lock lk(mu);
	auto is = data.find(key);
	return is == data.end() ? boost::optional<string>{} : is->second;
}