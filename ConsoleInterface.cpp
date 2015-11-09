#include "ConsoleInterface.h"

ConsoleInterface::ConsoleInterface() {
}

ConsoleInterface::~ConsoleInterface() {
}

ScreenState ConsoleInterface::GetState() const { return m_state; }

void ConsoleInterface::SetOptionList(std::vector<std::string>&& options) {
	m_options = options;
	m_selectedOption = 0;
	m_scrollOffset = 0;

	for (auto& str : m_options) 
		if (str.length() > ScreenWidth) 
			str.resize(ScreenWidth);

	Draw();
}

void ConsoleInterface::SetEntry(const PasswordEntry* entry) {
	m_entry = entry;
	Draw();
}

void ConsoleInterface::SwitchState(ScreenState state) {
	m_state = state;

	Draw();
}

void ConsoleInterface::SetObfuscateInput(bool b) { m_obfuscateInput = b; Draw(); }
void ConsoleInterface::SetFooter(const std::string& str) { m_footer = str; Draw(); }
void ConsoleInterface::SetHeader(const std::string& str) { m_header = str; Draw(); }

void ConsoleInterface::Draw() {
	m_screen.ClearScreenHighlight();
	int y = 0;

	// draw the header
	switch (m_state) {
	case VIEW_OPTION_LIST:
	case INPUT_STRING: {
			y = m_screen.WriteRow(y, m_header) + 1;
			break;
		}
	default:
		break;
	}

	// draw the footer
	m_screen.WriteRow(ScreenHeight - 1, m_footer);

	int availableRows = GetAvailableRows();

	switch (m_state) {
	case VIEW_OPTION_LIST: {
		for (int i = m_scrollOffset, j = 0; i < m_options.size() && i < availableRows; ++i, ++j) {
			const auto& str = m_options[i];
			m_screen.WriteRow(j + y, str);
		}

		m_screen.SetHighlightRow(m_selectedOption - m_scrollOffset + y, true);
	}
	default:
		break;
	}
}

int ConsoleInterface::GetAvailableRows() {
	return ScreenHeight - 3 - (m_header.length() / ScreenWidth + 1);
}

void ConsoleInterface::InputLoop() {
	while (true) {
		KeyPress keyPress;
		m_screen.ProcessInput(keyPress);
		std::string str;
		if (keyPress.isVirtual) {
			switch (keyPress.key.virtualCode) {
			case VK_RETURN:
				str += "return";
				break;
			case VK_BACK:
				str += "backspace";
				break;
			case VK_UP:
				str += "arrow up";
				break;
			case VK_DOWN:
				str += "arrow down";
				break;
			case VK_ESCAPE:
				return;
			}
		} else {
			str += keyPress.key.charCode;
		}
		SetFooter(str);
	}
}