#include <iostream>
#include <queue>
#include "buffered_channel.h"

template <class T>
Channel<T>::Channel(int buffer_size) {
		is_locked = false;
		is_closed = false;
		this->buffer_size = buffer_size;
		buffer = std::queue<T>();
	}

template <class T>
	void Channel<T>::send(T value) {
		if (!is_closed) {
			if (buffer_size == buffer.size()) {
				while (buffer_size == buffer.size()) {
					is_locked = true;
				}
				is_locked = false;
				buffer.push(value);
			}
			else {
				buffer.push(value);
			}
		}
		else {
			throw std::runtime_error("Can't read from closed channel");
		}
	}

	template <class T>
	std::pair<T, bool> Channel<T>::recv() {
		if (buffer.empty() && is_closed) {
			return std::make_pair<T(), false>;
		} else
		if(buffer.empty()){
			while (buffer.empty()) {
				is_locked = true;
			}
			is_locked = false;
			T result = buffer.front();
			buffer.pop();
			return std::make_pair<result, true>;
		}
		else {
			T result = buffer.front();
			buffer.pop();
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
}
