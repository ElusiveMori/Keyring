#include "ConsoleScreen.h"

ConsoleScreen::ConsoleScreen(ConsoleControls& controller) : m_controller(controller) {
	SECURITY_ATTRIBUTES attrib;
	attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
	attrib.bInheritHandle = false;
	attrib.lpSecurityDescriptor = NULL;

	if ((m_hConIn = CreateFile("CONIN$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, &attrib, OPEN_EXISTING, 0, 0)) == INVALID_HANDLE_VALUE) {
		throw std::runtime_error("Could not acquire the console input handle.");
	}

	if ((m_hOldScreenBuf = CreateFile("CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, &attrib, OPEN_EXISTING, 0, 0)) == INVALID_HANDLE_VALUE) {
		throw std::runtime_error("Could not acquire the active screen buffer.");
	}

	if ((m_hConOut = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, &attrib, CONSOLE_TEXTMODE_BUFFER, NULL)) == INVALID_HANDLE_VALUE) {
		throw std::runtime_error("Could not create the console screen buffer.");
	}

	if ((m_hConWindow = GetConsoleWindow()) == NULL) {
		throw std::runtime_error("Could not acquire the console screen window.");
	}

	GetConsoleMode(m_hConIn, &m_oldInMode);
	SetConsoleMode(m_hConIn, ENABLE_PROCESSED_INPUT | ENABLE_WINDOW_INPUT);
	SetConsoleMode(m_hConOut, 0);

	COORD size;
	size.X = ScreenWidth;
	size.Y = ScreenHeight;

	SMALL_RECT window;
	window.Bottom = ScreenHeight;
	window.Right = ScreenWidth;
	window.Top = 0;
	window.Left = 0;

	SetConsoleWindowInfo(m_hConOut, TRUE, &window);
	SetConsoleScreenBufferSize(m_hConOut, size);
	SetConsoleActiveScreenBuffer(m_hConOut);

	/* here we abuse the fact that the console window
	   won't allow to be larger than it's underlying
	   buffer allows, so it just sets it to the maximum
	   possible size */
	MoveWindow(m_hConWindow, 0, 0, 1000, 1000, TRUE);

	ClearScreenHighlight();
}

ConsoleScreen::~ConsoleScreen() {
	SetConsoleMode(m_hConIn, m_oldInMode);
	SetConsoleActiveScreenBuffer(m_hOldScreenBuf);
	CloseHandle(m_hOldScreenBuf);
	CloseHandle(m_hConIn);
	CloseHandle(m_hConOut);
}

ScreenState ConsoleScreen::GetState() const { return m_state; }

void ConsoleScreen::SetOptionList(std::vector<std::string>&& options) {
	m_options = options;
	Draw();
}

void ConsoleScreen::SetEntry(const PasswordEntry* entry) {
	m_entry = entry;
	Draw();
}

void ConsoleScreen::SwitchState(ScreenState state) {
	m_state = state;

	Draw();
}

void ConsoleScreen::SetObfuscateInput(bool b) { m_obfuscateInput = b; Draw(); }
void ConsoleScreen::SetFooter(const std::string& str) { m_footer = str; Draw(); }
void ConsoleScreen::SetHeader(const std::string& str) { m_header = str; Draw(); }

void ConsoleScreen::Draw() {
	int x = 0, y = 0;

	DWORD written;
	COORD pos; pos.X = 0; pos.Y = 0;
	/* draw the header */
	switch (m_state) {
	case VIEW_OPTION_LIST:
	case INPUT_STRING: {
			y = WriteRow(y, m_header) + 1;
			break;
		}
	default:
		break;
	}

	/* draw the footer */
	WriteRow(ScreenHeight - 1, m_footer);

	switch (m_state) {
	case VIEW_OPTION_LIST:
	default:
		break;
	}
}

int ConsoleScreen::WriteRow(int row, const std::string& str) {
	int rows = str.length() / ScreenWidth;
	DWORD written;
	COORD pos; pos.X = 0; pos.Y = row;
	FillConsoleOutputAttribute(m_hConOut, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY, rows * ScreenWidth, pos, &written);
	WriteConsoleOutputCharacter(m_hConOut, str.c_str(), str.length(), pos, &written);
	return rows;
}

void ConsoleScreen::SetHighlightRow(int row, bool b) {
	DWORD flag = b ? (BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY) : (FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	DWORD written;
	COORD pos; pos.X = 0; pos.Y = row;
	FillConsoleOutputAttribute(m_hConOut, flag, ScreenWidth, pos, &written);
}

void ConsoleScreen::ClearRow(int row) {
	DWORD written;
	COORD pos; pos.X = 0; pos.Y = row;
	FillConsoleOutputCharacter(m_hConOut, ' ', ScreenWidth, pos, &written);
	SetHighlightRow(row, false);
}

void ConsoleScreen::ClearScreenHighlight() {
	DWORD written;
	COORD pos; pos.X = 0; pos.Y = 0;
	FillConsoleOutputAttribute(m_hConOut, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY, ScreenWidth * ScreenHeight, pos, &written);
}