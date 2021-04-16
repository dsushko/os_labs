#pragma once

template <class T>

class Channel {
private:
	bool is_locked;
	bool is_closed;
	std::queue<T> buffer;
	int buffer_size;
public:
	Channel(int buffer_size);

	void send(T value);

	std::pair<T, bool> recv();

	void close();
};