#include <iostream>
#include "KeyRing.h"
#include "ConsoleInterface.h"
#include <boost/filesystem.hpp>

int main() {
	KeyRing key;

	key.PushState(new ConState::LoadDB(key));
	key.GetInterface().Draw();
	key.GetInterface().InputLoop();
}