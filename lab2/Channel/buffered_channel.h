#pragma once

#include <queue>

template <class T>
class BufferedChannel {
private:
	bool is_locked;
	bool is_closed;
	std::queue<T> buffer;
	int buffer_size;
public:
	BufferedChannel(int buffer_size);

	void send(T value);

	std::pair<T, bool> recv();

	void close();
};