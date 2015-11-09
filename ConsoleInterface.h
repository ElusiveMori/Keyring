#pragma once

#include "Constants.h"
#include "PasswordManager.h"
#include "ConsoleScreen.h"
#include <string>
#include <vector>
#include <Windows.h>

class ConsoleInterface {
	/* screen wrapper */
	ConsoleScreen m_screen;

	/* current screen state */
	ScreenState m_state = INPUT_STRING;

	std::string m_header; /* displayed at the top of the screen */
	std::string m_footer; /* displayed at the bottom of the screen */

	/* used when screen state is VIEW_OPTION_LIST */
	std::vector<std::string> m_options;

	/* selected option id - this option will be highlighted */
	int m_selectedOption = 0;
	int m_scrollOffset = 0;

	/* used when the screen state is INPUT_STRING
	   acts as the input buffer and can be accessed
	   by the managing class */
	std::string m_input;

	/* used when the screen state is INPUT_STRING
	   if this is true, then the input string will
	   be shown as asterisks to the user instead of the
	   actual contents */
	bool m_obfuscateInput = false;

	/* used when the screen state is VIEW_ENTRY 
	   this is the entry being viewed */
	const PasswordEntry* m_entry = nullptr;

	/* returns the amount of rows available
	   between the header and the footer
	   including margins */
	int GetAvailableRows();
public:
	ScreenState GetState() const;
	void SwitchState(ScreenState);
	void SetObfuscateInput(bool);
	void SetFooter(const std::string&);
	void SetHeader(const std::string&);

	/* redraws the screen
	 * based on the screen state, this will
	 * will produce different results */ 
	void Draw();

	/* sets the option list that will be displayed */
	void SetOptionList(std::vector<std::string>&&);

	/* sets the entry that will be displayed */
	void SetEntry(const PasswordEntry*);

	void InputLoop();

	ConsoleInterface();
	~ConsoleInterface();
};