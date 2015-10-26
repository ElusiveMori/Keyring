#pragma once

#include <cstdint>
#include <string>
#include <iostream>

template <typename T>
T ReadType(std::istream&);

template <typename T>
void WriteType(std::ostream&, T);