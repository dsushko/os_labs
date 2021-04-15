#pragma once
#include <mutex>
#include <queue>

template <class T>
class Channel {
private:
	std::mutex recv_locker;
	std::mutex send_locker;
	bool is_closed;
	std::queue<T> buffer;
	int buffer_size;
public:
	Channel(int buffer_size);

	void send(T value);

	std::pair<T, bool> recv();

	void close();
};