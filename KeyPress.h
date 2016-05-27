#pragma once
#include <Windows.h>

struct KeyPress {
	bool isVirtual;
	union {
		CHAR charCode;
		WORD virtualCode;
	} key;
};