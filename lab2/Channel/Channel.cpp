#include <iostream>
#include <queue>
#include "buffered_channel.h"

template <class T>
Channel<T>::Channel(int buffer_size) {
	is_closed = false;
	this->buffer_size = buffer_size;
	buffer = std::queue<T>();
}

template <class T>
void Channel<T>::send(T value) {
	send_locker.lock();
	if (!is_closed) {
		if (buffer_size == buffer.size()) {
			while (buffer_size == buffer.size()) {

			}
			buffer.push(value);
		}
		else {
			buffer.push(value);
		}
	}
	else {
		send_locker.unlock();
		throw std::runtime_error("Can't read from closed channel");
	}
	send_locker.unlock();
}

template <class T>
std::pair<T, bool> Channel<T>::recv() {
	recv_locker.lock();
	if (buffer.empty() && is_closed) {
		recv_locker.unlock();
		return std::make_pair<T(), false>;
	}
	else
		if (buffer.empty()) {
			while (buffer.empty()) {

			}
			T result = buffer.front();
			buffer.pop();

			recv_locker.unlock();
			return std::make_pair<result, true>;
		}
		else {
			T result = buffer.front();
			buffer.pop();

			recv_locker.unlock();
			return std::make_pair<result, true>;
		}
}

template <class T>
void Channel<T>::close() {
	is_closed = true;
}

int main()
{
	std::cout << "Hello World!\n";

	return 0;
}
