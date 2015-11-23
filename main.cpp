#include <iostream>
#include "KeyRing.h"
#include "ConsoleInterface.h"

int main() {
	ConsoleInterface con;

	std::vector<std::string> vec;

	for (int i = 0; i < 100; i++) vec.push_back(std::to_string(i) + ". Something.");

	con.SetOptionList(std::move(vec));
	con.SwitchState(VIEW_OPTION_LIST);

	con.InputLoop();
}