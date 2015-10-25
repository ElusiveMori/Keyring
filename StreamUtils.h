#pragma once

#include <cstdint>
#include <string>
#include <iostream>

namespace StreamUtils {
	template <typename T>
	T ReadType(std::istream&);

	template <typename T>
	void WriteType(std::ostream&, T);

	std::string ReadString(std::istream&);
	void WriteString(std::ostream&);
}