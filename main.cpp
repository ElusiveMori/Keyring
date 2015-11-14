#include <iostream>
#include "KeyRing.h"
#include "ConsoleInterface.h"

int main() {
	PasswordManager pm;
	pm.AddEntry("1", "2", "3");
	pm.AddEntry("1", "2", "3");
	pm.AddEntry("1", "2", "3");
	pm.AddEntry("1", "2", "3");

	std::stringstream str;

	Security::SaveManager(str, pm, "pootis");
	
	PasswordManager pm2;

	Security::LoadManager(str, pm2, "pootis");
}