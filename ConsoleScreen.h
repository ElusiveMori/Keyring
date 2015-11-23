#pragma once

#include <Windows.h>
#include <string>

struct KeyPress {
	bool isVirtual;
	union {
		CHAR charCode;
		WORD virtualCode;
	} key;
};

class ConsoleScreen {
	/* previous console input mode, class restores to this after being destroyed */
	DWORD m_oldInMode = 0;

	/* handle to the previous screen buffer of the active console
	the class restores the active screen buffer to this one
	after being destroyed */
	HANDLE m_hOldScreenBuf = NULL;

	/* handle to the console input */
	HANDLE m_hConIn = NULL;

	/* I use two buffers here to circumvent screen flickering
	   while drawing to the screen. m_hConOut is the buffer
	   that's currently being drawn to. After a single draw
	   operation completes, the active screen buffer is set
	   to m_hConOut, and these handles are swapped */

	HANDLE m_hConOut = NULL;
	HANDLE m_hConBackBuf = NULL;

	/* handle to the console screen window */
	HWND m_hConWindow = NULL;
public:
	/* writes a string starting at the specified row
	returns the number of rows written to */
	int WriteRow(int, const std::string&);

	/* clears the specified row */
	void ClearRow(int);

	/* sets whether a row is highlighted or not */
	void SetHighlightRow(int, bool);

	/* resets the screen highlighting */
	void ClearScreenHighlight();

	void HideCursor();

	/* processes input and writes a single key
	   to the passed variable */
	void ProcessInput(KeyPress&);

	/* swaps m_hConOut and m_hConBackBuf */
	void SwapBuffers();

	ConsoleScreen();
	~ConsoleScreen();
};

