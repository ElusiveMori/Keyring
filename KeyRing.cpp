#include "KeyRing.h"
#include "ConsoleInterface.h"
#include <iostream>
#include <cstdint>

KeyRing::KeyRing() : m_manager()
{
	ConsoleInterface ui(&m_manager);
}


KeyRing::~KeyRing()
{
}
