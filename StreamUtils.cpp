#include "StreamUtils.h"
#include <memory>

template <typename T>
T ReadType(std::istream& stream) {
	static_assert(std::is_arithmetic<T>, "Type is not arithmetic.");
	T read;
	if (stream.read(reinterpret_cast<char*>(&read), sizeof(T)))
		return read;
	else
		throw std::runtime_error("Stream is in a bad state.");
}

template<>
std::string ReadType(std::istream& stream) {
	uint16_t size = ReadType<uint16_t>(stream);
	std::unique_ptr<char[]> data(new char[size]);
	if (stream.read(data.get(), size))
		return std::string(data.get(), size);
	else
		throw std::runtime_error("Stream is in a bad state.");

}

template <typename T>
void WriteType(std::ostream& stream, T write) {
	static_assert(std::is_arithmetic<T>, "Type is not arithmetic.");
	if (!stream.write(reinterpret_cast<char*>(&write), sizeof(T)))
		throw std::runtime_error("Stream is in a bad state.")
}

template<>
void WriteType(std::ostream& stream, std::string string) {
	WriteType<uint16_t>(stream, string.size());
	if (!stream.write(string.c_str(), string.size()))
		throw std::runtime_error("Stream is in a bad state.");
}