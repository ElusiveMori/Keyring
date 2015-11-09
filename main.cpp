#include <iostream>
#include "KeyRing.h"
#include "ConsoleInterface.h"

int main() {
	ConsoleInterface c;
	c.SetHeader("Header.");
	c.SetFooter("Footer.");
	c.SetOptionList({ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12" });
	c.SwitchState(VIEW_OPTION_LIST);
	c.Draw();

	c.InputLoop();
}