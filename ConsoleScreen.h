#pragma once

#include "Enums.h"
#include <string>
#include <vector>
#include "PasswordManager.h"
#include <Windows.h>

class ConsoleControls;

const int ScreenWidth = 80;
const int ScreenHeight = 25;

class ConsoleScreen {
	/* current screen state */
	ScreenState m_state = INPUT_STRING;

	/* reference to the object that's meant to be controlling this class */
	ConsoleControls& m_controller;

	/* previous console input mode, class restores to this after being destroyed */
	DWORD m_oldInMode = 0;
	
	/* handle to the previous screen buffer of the active console
	   the class restores the active screen buffer to this one
	   after being destroyed */
	HANDLE m_hOldScreenBuf = NULL;

	/* handle to the console input */
	HANDLE m_hConIn = NULL;

	/* handle to our own screen buffer */
	HANDLE m_hConOut = NULL;

	/* handle to the console screen window */
	HWND m_hConWindow = NULL;

	std::string m_header = ""; /* displayed at the top of the screen */
	std::string m_footer = ""; /* displayed at the bottom of the screen */

	/* used when screen state is VIEW_OPTION_LIST */
	std::vector<std::string> m_options = std::vector<std::string>();

	/* used when the screen state is INPUT_STRING
	   acts as the input buffer and can be accessed
	   by the managing class */
	std::string m_input = std::string();

	/* used when the screen state is INPUT_STRING
	   if this is true, then the input string will
	   be shown as asterisks to the user instead of the
	   actual contents */
	bool m_obfuscateInput = false;

	/* used when the screen state is VIEW_ENTRY 
	   this is the entry being viewed */
	const PasswordEntry* m_entry = nullptr;

	/* writes a string starting at the specified row
	   returns the number of rows written to */
	int WriteRow(int, const std::string&);

	/* clears the specified row */
	void ClearRow(int);

	/* sets whether a row is highlighted or not */
	void SetHighlightRow(int, bool);

	/* resets the screen highlighting */
	void ClearScreenHighlight();
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

	ConsoleScreen(ConsoleControls&);
	~ConsoleScreen();
};

