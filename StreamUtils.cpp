#include "StreamUtils.h"

template <typename T>
T ReadType(std::iostream& stream) {
	static_assert(std::is_arithmetic<T>, "Type is not arithmetic.");
	T read;
	if (stream.read(reinterpret_cast<char*>(&read), sizeof(T)))
		return read;
	else
		throw std::runtime_error("Stream is in a bad state.")
}

template <typename T>
void WriteType(std::iostream& stream, T write) {
	static_assert(std::is_arithmetic<T>, "Type is not arithmetic.");
	if (!stream.write(reinterpret_cast<char*>(&write), sizeof(T)))
		throw std::runtime_error("Stream is in a bad state.")
}